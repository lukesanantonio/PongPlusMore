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
#include "ControlInterface.h"
#include <cstdlib>
#include "json/json.h"
namespace pong
{
  namespace detail
  {
    ServerAction parse(Json::Value root)
    {
      std::string method = root["method"].asString();
      if(method == "Server.DeleteObject")
      {
        ObjectDeletionAction a;
        a.id = root["params"][0].asInt();
        return ServerAction{a};
      }
      return NullAction{};
    }
  }
  ServerAction compile_command(const std::vector<char>& buf) noexcept
  {
    Json::Reader reader(Json::Features::strictMode());
    Json::Value req;
    if(!reader.parse(std::string(buf.begin(), buf.end()), req))
    {
      // Log the parse error.
      std::string str_buf(buf.begin(), buf.end());
      return LogAction{severity::warning,
                       "Failed to compile command from: '" + str_buf + "'"};
    }

    return detail::parse(req);
  }

  struct Pipe
  {
    uv_pipe_t pipe;
    std::vector<char>* buf;
    Server* s;
  };

  Pipe* create_pipe(Server& s) noexcept
  {
    Pipe* t = new Pipe;
    std::memset(t, 0, sizeof(Pipe));

    t->buf = new std::vector<char>();
    t->s = &s;

    return t;
  }

  void delete_pipe(Pipe* t) noexcept
  {
    delete t->buf;
    delete t;
  }

  struct Process
  {
    uv_process_t proc;
    Pipe* write_pipe;
    Pipe* read_pipe;
    Pipe* error_pipe;
    Server* s;
  };

  void delete_process(Process* self)
  {
    if(self->error_pipe)
    {
      uv_close((uv_handle_t*) &self->error_pipe->pipe, NULL);
      delete_pipe(self->error_pipe);
      self->error_pipe = nullptr;
    }

    if(self->read_pipe)
    {
      uv_close((uv_handle_t*) &self->read_pipe->pipe, NULL);
      delete_pipe(self->read_pipe);
      self->read_pipe = nullptr;
    }

    if(self->write_pipe)
    {
      uv_close((uv_handle_t*) &self->write_pipe->pipe, NULL);
      delete_pipe(self->write_pipe);
      self->write_pipe = nullptr;
    }

    delete self;
  }

  void on_process_exit(uv_process_t* self, int64_t exit, int sig) noexcept
  {
    Process* self_process = (Process*) self;
    self_process->s->log(severity::info,
                         "Process " + std::to_string(self_process->proc.pid) +
                         " (PID) finished.");

    uv_close((uv_handle_t*) self_process, NULL);
    delete_process(self_process);
  }

  Process* create_process(char** args, uv_loop_t* loop, Server& s)
  {
    // Allocate memory.
    Process* self = new Process();

    // Init the child process' stdin.
    self->write_pipe = create_pipe(s);
    uv_pipe_init(loop, &self->write_pipe->pipe, 1);

    // Init the child process' stdout.
    self->read_pipe = create_pipe(s);
    uv_pipe_init(loop, &self->read_pipe->pipe, 1);

    // Init the child process' stderr.
    self->error_pipe = create_pipe(s);
    uv_pipe_init(loop, &self->error_pipe->pipe, 1);

    // Record the server spawning the process.
    self->s = &s;

    // Set up spawn parameters.
    uv_process_options_t options = {0};

    options.stdio_count = 3;

    uv_stdio_container_t stdio[3];

    stdio[0].flags = uv_stdio_flags(UV_CREATE_PIPE | UV_READABLE_PIPE);
    stdio[0].data.stream = (uv_stream_t*) &self->write_pipe->pipe;

    stdio[1].flags = uv_stdio_flags(UV_CREATE_PIPE | UV_WRITABLE_PIPE);
    stdio[1].data.stream = (uv_stream_t*) &self->read_pipe->pipe;

    stdio[2].flags = uv_stdio_flags(UV_CREATE_PIPE | UV_WRITABLE_PIPE);
    stdio[2].data.stream = (uv_stream_t*) &self->error_pipe->pipe;

    options.stdio = stdio;

    options.file = args[0];
    options.args = args;

    options.exit_cb = on_process_exit;

    int err = uv_spawn(loop, (uv_process_t*) self, &options);
    if(err)
    {
      delete_process(self);
      s.log(severity::error, "Failed to spawn '" +
                             std::string(args[0]) + "': " + uv_strerror(err));
      return nullptr;
    }

    s.log(severity::info,
          "Successfully spawned '" + std::string(args[0]) + "' with PID = " +
          std::to_string(self->proc.pid));
    return self;
  }

  void enqueue_buffer(Pipe* pipe) noexcept
  {
    // Compile the command.
    ServerAction a = compile_command(*pipe->buf);
    // Queue the command.
    pipe->s->enqueue_action(a);
    // Clear the buffer.
    pipe->buf->clear();
  }

  void collect_commands(uv_stream_t* stream,
                        ssize_t nread, const uv_buf_t* buf)
  {
    Pipe* pipe = (Pipe*) stream;

    if(nread == UV_EOF)
    {
      // Send off the buffer, even if it may not be complete.
      enqueue_buffer(pipe);

      // Stop reading.
      uv_read_stop(stream);
      return;
    }

    for(char* cur = buf->base; cur != buf->base + nread; ++cur)
    {
      if(*cur == '\n')
      {
        // Enqueue the command, then continue on to the next line.
        enqueue_buffer(pipe);
        continue;
      }
      pipe->buf->push_back(*cur);
    }

    // Deallocate the old buffer.
    delete[] buf->base;
  }

  void alloc(uv_handle_t* handle, size_t ssize, uv_buf_t* buf)
  {
    buf->base = new char[ssize];
    buf->len = ssize;
  }

  struct WriteReq
  {
    uv_write_t req;
    Pipe** pipe;
  };
  void on_finish_write(uv_write_t* req, int status)
  {
    WriteReq* write = (WriteReq*) req;

    // Close the pipe.
    uv_close((uv_handle_t*) &(*write->pipe)->pipe, NULL);

    // And delete it!
    delete_pipe(*write->pipe);

    // Then null it out.
    *write->pipe = nullptr;

    // Deallocate the entire structure.
    delete write;
  }

  void spawn_plugin(Server& s, std::vector<std::string> args, uv_loop_t* loop)
  {
    // Make a char** from the vector.
    std::vector<char*> str_args;
    for(auto& arg : args)
    {
      str_args.push_back(&arg[0]);
    }
    str_args.push_back(NULL);

    Process* p = create_process(&str_args[0],  loop, s);

    // Set read callbacks
    uv_read_start((uv_stream_t*) &p->read_pipe->pipe, alloc, collect_commands);
    //uv_read_start(p->error_pipe, alloc, collect_commands);No stderr just yet.

    // Tell the process to begin.
    // TODO: Inform the child process of Server state here.
    WriteReq* req = new WriteReq;
    req->pipe = &p->write_pipe;

    uv_buf_t buf;
    buf.base = (char*) "PPM";
    buf.len = std::strlen(buf.base);
    uv_write((uv_write_t*) req, (uv_stream_t*) &p->write_pipe->pipe,
             &buf, 1, on_finish_write);
  }
}
