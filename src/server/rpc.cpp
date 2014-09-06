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
#include "rpc.h"
#include "ipc.h"
#include "server_actions.h"
#include "common/serialize.h"
#include "common/deserialize.h"
#include "common/util.h"
namespace pong
{
  namespace detail
  {
    ServerAction parse(Json::Value root, Pipe* out)
    {
      // Get the method.
      std::string method = root["method"].asString();

      // Get the request id, if applicable.
      id_type request_id = 0;
      if(root["id"].isInt())
      {
        request_id = root["id"].asInt();
      }

      if(method == "Server.CreateObject")
      {
        ObjectCreationAction a = parse_create_action(root["params"]);
        a.callback = [request_id, out](id_type obj_id)
        {
          if(obj_id)
          {
            std::string res = parse_string(make_result(request_id, obj_id));
            *out->buf = vec_from_string(res);
          }
          else
          {
            std::string err_msg = "Failed to create object";
            std::string res = parse_string(make_error(request_id, err_msg));
            *out->buf = vec_from_string(res);
          }
          write_buffer(out);
        };
      }
      if(method == "Server.DeleteObject")
      {
        return parse_delete_action(root["params"]);
      }
      if(method == "Server.Log")
      {
        return parse_log_action(root["params"]);
      }

      return NullAction{};
    }
  }
  ServerAction compile_buffer(Pipe* src, Pipe* out) noexcept
  {
    Json::Reader reader(Json::Features::strictMode());
    Json::Value req;

    std::string doc(src->buf->begin(), src->buf->end());
    if(!reader.parse(doc, req))
    {
      // Log the parse error.
      return LogAction{Severity::Warning,
                       "Failed to compile command: '" + doc + "'"};
    }

    // Parse the request object.
    return detail::parse(req, out);
  }

  void enqueue_buffer(Pipe* pipe, Pipe* out_pipe) noexcept
  {
    // Don't do anything if the pipe doesn't have any command buffered!
    if(pipe->buf->empty()) return;

    // Compile the command.
    ServerAction a = compile_buffer(pipe, out_pipe);
    // Queue the command.
    pipe->proc->server->enqueue_action(a);
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
      enqueue_buffer(pipe, pipe->proc->out_pipe);

      // Stop reading.
      uv_read_stop(stream);
      return;
    }

    for(char* cur = buf->base; cur != buf->base + nread; ++cur)
    {
      if(*cur == '\n')
      {
        // Enqueue the command, then continue on to the next line.
        enqueue_buffer(pipe, pipe->proc->out_pipe);
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

  void on_finish_write(uv_write_t* req, int status)
  {
    delete req;
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

    SpawnOptions opts;
    opts.args = &str_args[0];
    opts.cwd = NULL;
    Process* p = create_process(loop, opts, s);

    // Start the boll rollin'!

    // Set read callbacks
    uv_read_start((uv_stream_t*) p->in_pipe, alloc, collect_commands);
    //uv_read_start(p->error_pipe, alloc, collect_commands);No stderr just yet.

    // Tell the process to begin.
    // TODO: Inform the child process of Server state here.
    uv_write_t* req = new uv_write_t;

    *p->out_pipe->buf = vec_from_string("PPM");
    write_buffer(p->out_pipe);
  }
}
