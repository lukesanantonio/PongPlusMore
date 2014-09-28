/*
 * PpM - Pong Plus More - A pong clone full of surprises written with C++11.
 * Copyright (C) 2013  Luke San Antonio
 *
 * You can contact me (Luke San Antonio) at lukesanantonio@gmail.com!
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "ipc.h"
#include <string>
#include "Logger.h"
namespace pong
{
  // Pipe initialization functions.
  Pipe* create_pipe(Process* proc) noexcept
  {
    Pipe* self = new Pipe;
    init_pipe(*self);
    return self;
  }
  void delete_pipe(Pipe* self) noexcept
  {
    uninit_pipe(*self);
    delete self;
  }
  void init_pipe(Pipe& self, Process* proc) noexcept
  {
    self.buf = new std::vector<char>();
    self.proc = proc;
  }

  void uninit_pipe(Pipe& self) noexcept
  {
    delete self.buf;
  }

  // DuplexPipe initialization functions.
  DuplexPipe* create_duplex_pipe(Process* proc) noexcept
  {
    DuplexPipe* self = new DuplexPipe;
    init_duplex_pipe(*self, proc);
    return self;
  }
  void delete_duplex_pipe(DuplexPipe* self) noexcept
  {
    uninit_duplex_pipe(*self);
    delete self;
  }

  void init_duplex_pipe(DuplexPipe& self, Process* proc) noexcept
  {
    // The pipes are already allocated, so we just need to initialize them.
    init_pipe(self.in, proc);
    init_pipe(self.out, proc);
  }
  void uninit_duplex_pipe(DuplexPipe& self) noexcept
  {
    uninit_pipe(self.in);
    uninit_pipe(self.out);
  }

  void on_process_exit(uv_process_t* p, int64_t exit, int sig) noexcept
  {
    Process* proc = (Process*) p;

    if(sig)
    {
      proc->log->log(Severity::Info,
                     "Process killed (PID: " + std::to_string(proc->proc.pid) +
                     ", Exit Status: " + std::to_string(exit) + ", Signal: " +
                     std::to_string(sig) + ")");
    }
    else
    {
      proc->log->log(Severity::Info,
                     "Process exited normally (PID: " +
                     std::to_string(proc->proc.pid) + ", Exit Status: " +
                     std::to_string(exit) + ")");
    }

    // Tidy up.
    uv_run(proc->loop, UV_RUN_DEFAULT);

    // Close the process then uninitialize.
    uv_close((uv_handle_t*) proc, NULL);
    delete_process(proc);
  }

  Process* create_process(uv_loop_t* loop,
                          const SpawnOptions& spawn_opt,
                          Logger& l) noexcept
  {
    Process* self = new Process;
    self->log = &l;
    self->loop = loop;
    self->running = false;

    // Initialize pipes.
    init_duplex_pipe(self->io, self);
    init_pipe(self->err, self);

    uv_pipe_init(loop, (uv_pipe_t*) &self->io.in, 1);
    uv_pipe_init(loop, (uv_pipe_t*) &self->io.out, 1);
    uv_pipe_init(loop, (uv_pipe_t*) &self->err, 1);

    uv_process_options_t options = {0};

    // Specify executable.
    options.file = spawn_opt.args[0];
    options.args = spawn_opt.args;
    options.cwd = spawn_opt.cwd;

    options.stdio_count = 3;
    uv_stdio_container_t stdio[3];

    // Set up pipes to connect to the child's stdin, stdout, and stderr.
    stdio[0].flags = uv_stdio_flags(UV_CREATE_PIPE | UV_READABLE_PIPE);
    stdio[0].data.stream = (uv_stream_t*) &self->io.out;

    stdio[1].flags = uv_stdio_flags(UV_CREATE_PIPE | UV_WRITABLE_PIPE);
    stdio[1].data.stream = (uv_stream_t*) &self->io.in;

    stdio[2].flags = uv_stdio_flags(UV_CREATE_PIPE | UV_WRITABLE_PIPE);
    stdio[2].data.stream = (uv_stream_t*) &self->err;

    options.stdio = stdio;

    // This callback closes the process handle and then calles delete_process.
    options.exit_cb = on_process_exit;

    std::string proc_name(spawn_opt.args[0]);

    int err = uv_spawn(loop, (uv_process_t*) self, &options);
    if(err)
    {
      self->log->log(Severity::Error,
                     "Failed to spawn process: '" + proc_name +"' (Error: '" +
                     uv_strerror(err) + "')");
      delete_process(self);
      return nullptr;
    }

    self->running = true;

    self->log->log(Severity::Info,
                   "Successfully spawned process (Process Name: '" +
                   proc_name + "', PID: " +
                   std::to_string(self->proc.pid) + ")");
    return self;
  }

  void delete_process(Process* self) noexcept
  {
    // Kill the process (if necessary)
    kill_process(self, SIGTERM);

    // Tidy up all pending events.
    uv_run(self->loop, UV_RUN_DEFAULT);

    // Close and uninitialize pipes connected to the child process.
    uv_close((uv_handle_t*) &self->io.in, NULL);
    uninit_pipe(self->io.in);

    uv_close((uv_handle_t*) &self->io.out, NULL);
    uninit_pipe(self->io.out);

    uv_close((uv_handle_t*) &self->err, NULL);
    uninit_pipe(self->err);

    delete self;
  }

  void kill_process(Process* proc, int signum) noexcept
  {
    if(!proc->running) return;
    int err = uv_process_kill((uv_process_t*) proc, signum);
    if(err && err != UV_ESRCH)
    {
      proc->log->log(Severity::Error, "Failed to kill process (PID: " +
                     std::to_string(proc->proc.pid) + ", Signal: " +
                     std::to_string(signum) + ")");
    }
  }

  struct WriteBufReq
  {
    uv_write_t req;
    Pipe* pipe;
    on_write_cb after_write;
  };

  void on_write_buffer(uv_write_t* r, int status)
  {
    WriteBufReq* req = (WriteBufReq*) r;
    if(req->after_write) req->after_write(req->pipe);
    delete req;
  }
  void write_buffer(Pipe* pipe, on_write_cb after_write) noexcept
  {
    uv_buf_t buf = uv_buf_init(&(*pipe->buf)[0], pipe->buf->size());

    WriteBufReq* req = new WriteBufReq;
    req->pipe = pipe;
    req->after_write = after_write;

    uv_write((uv_write_t*) req, (uv_stream_t*) pipe, &buf, 1, on_write_buffer);
  }

  void collect_lines(uv_stream_t* s, ssize_t nread, const uv_buf_t* buf)
  {
    Pipe* p = (Pipe*) s;
    if(nread == UV_EOF)
    {
      // Call the action as long as the buffer is not empty.
      if(!p->buf->empty()) { p->action_cb(p); }
      uv_read_stop(s);
    }
    else
    {
      for(char* cur = buf->base; cur != buf->base + nread; ++cur)
      {
        if(*cur == '\n')
        {
          if(!p->buf->empty()) { p->action_cb(p); }

          // Clear the buffer.
          p->buf->clear();
        }
        else
        {
          p->buf->push_back(*cur);
        }
      }
    }
    delete[] buf->base;
  }
}
