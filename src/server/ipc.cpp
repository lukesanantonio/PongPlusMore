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
namespace pong
{
  Pipe* create_pipe(Process* proc) noexcept
  {
    Pipe* self = new Pipe;
    self->buf = new std::vector<char>();
    self->proc = proc;
    return self;
  }
  void delete_pipe(Pipe* self) noexcept
  {
    delete self->buf;
    delete self;
  }

  void on_process_exit(uv_process_t* p, int64_t exit, int sig) noexcept
  {
    Process* proc = (Process*) p;

    Server& s = *proc->server;
    s.logger().log(Severity::Info, "Process " +
                                   std::to_string(proc->proc.pid) +
                                   " (PID) returned with status " +
                                   std::to_string(exit));

    // Close the process then uninitialize.
    uv_close((uv_handle_t*) proc, NULL);
    delete_process(proc);
  }

  Process* create_process(uv_loop_t* loop,
                          const SpawnOptions& spawn_opt,
                          Server& server) noexcept
  {
    Process* self = new Process;
    self->server = &server;

    // Allocate the pipes.
    self->out_pipe = create_pipe(self);
    self->in_pipe = create_pipe(self);
    self->err_pipe = create_pipe(self);

    // Initialize pipes.
    uv_pipe_init(loop, (uv_pipe_t*) self->out_pipe, 1);
    uv_pipe_init(loop, (uv_pipe_t*) self->in_pipe, 1);
    uv_pipe_init(loop, (uv_pipe_t*) self->err_pipe, 1);

    uv_process_options_t options = {0};

    // Specify executable.
    options.file = spawn_opt.args[0];
    options.args = spawn_opt.args;
    options.cwd = spawn_opt.cwd;

    options.stdio_count = 3;
    uv_stdio_container_t stdio[3];

    // Set up pipes to connect to the child's stdin, stdout, and stderr.
    stdio[0].flags = uv_stdio_flags(UV_CREATE_PIPE | UV_READABLE_PIPE);
    stdio[0].data.stream = (uv_stream_t*) self->out_pipe;

    stdio[1].flags = uv_stdio_flags(UV_CREATE_PIPE | UV_WRITABLE_PIPE);
    stdio[1].data.stream = (uv_stream_t*) self->in_pipe;

    stdio[2].flags = uv_stdio_flags(UV_CREATE_PIPE | UV_WRITABLE_PIPE);
    stdio[2].data.stream = (uv_stream_t*) self->err_pipe;

    options.stdio = stdio;

    // This callback closes the process handle and then calles delete_process.
    options.exit_cb = on_process_exit;

    std::string proc_name(spawn_opt.args[0]);

    int err = uv_spawn(loop, (uv_process_t*) self, &options);
    if(err)
    {
      server.logger().log(Severity::Error,
                          "Failed to spawn process: '" + proc_name +"'");
      delete_process(self);
      return nullptr;
    }

    server.logger().log(Severity::Info,
                        "Successfully spawned: '" + proc_name +
                        "' with PID = " + std::to_string(self->proc.pid));
    return self;
  }

  void delete_process(Process* self) noexcept
  {
    auto uninit_pipe = [](Pipe** pipe)
    {
      if(!*pipe) return;

      uv_close((uv_handle_t*) *pipe, NULL);
      delete_pipe(*pipe);
      *pipe = nullptr;
    };

    // Uninitialize pipes to the child process.
    uninit_pipe(&self->out_pipe);
    uninit_pipe(&self->in_pipe);
    uninit_pipe(&self->err_pipe);

    delete self;
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
}
