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
#include "req.h"
#include "common/serialize.h"
#include "common/deserialize.h"
#include "common/util.h"
namespace pong
{
  net::req::Request create_request(Json::Value const& root) noexcept
  {
    const std::string& method = root["method"].asString();

    net::req::Request_Base base;
    base.id = root["id"].asInt();

    if(method == net::req::Log::method_name)
    {
      return net::req::Log(base);
    }
    if(method == net::req::CreateObject::method_name)
    {
      return net::req::CreateObject(base);
    }
    if(method == net::req::DeleteObject::method_name)
    {
      return net::req::DeleteObject(base);
    }
    else
    {
      return net::req::Null(base);
    }
  }

  net::req::Request fill_request(Json::Value const& root,
                                 DuplexPipe* pipe) noexcept
  {
    net::req::Request req = create_request(root);

    struct Request_Filler : public boost::static_visitor<void>
    {
      Request_Filler(Json::Value const& root, DuplexPipe* pipe)
                     : root_(root), pipe_(pipe) {}
      void operator()(net::req::Null&) const {}
      void operator()(net::req::Log& req) const
      {
        req.severity = parse_severity(this->root_["params"][0]);
        req.msg = this->root_["params"][1].asString();
      }
      void operator()(net::req::CreateObject& req) const
      {
        req.obj = parse_object(this->root_["params"][0]);
        DuplexPipe* pipe = this->pipe_;
        req.callback = [&req, pipe](id_type obj_id)
        {
          if(obj_id)
          {
            std::string res = parse_string(make_result(req.id, obj_id));
            *pipe->out.buf = vec_from_string(res);
          }
          else
          {
            std::string err_msg = "Failed to create object";
            std::string res = parse_string(make_error(req.id, err_msg));
            *pipe->out.buf = vec_from_string(res);
          }
          write_buffer(&pipe->out);
        };
      }
      void operator()(net::req::DeleteObject& req) const
      {
        req.obj_id = this->root_["params"][0].asInt();
      }
    private:
      Json::Value const& root_;
      DuplexPipe* pipe_;
    };

    boost::apply_visitor(Request_Filler(root, pipe), req);
    return req;
  }

  net::req::Request parse_buffer(DuplexPipe* pipe) noexcept
  {
    Json::Reader reader(Json::Features::strictMode());
    Json::Value req;

    std::string doc(pipe->in.buf->begin(), pipe->in.buf->end());
    if(!reader.parse(doc, req))
    {
      // Log the parse error.
      net::req::Log req;
      req.severity = Severity::Warning;
      req.msg = "Failed to compile command: '" + doc + "'";
      return req;
    }

    // Parse the request object.
    return fill_request(req, pipe);
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

  void compile_buffer(Pipe* p) noexcept
  {
    p->proc->server->enqueue_request(parse_buffer((DuplexPipe*) p));
  }
  void log_buffer(Pipe* p) noexcept
  {
    std::string msg(p->buf->begin(), p->buf->end());
    p->proc->server->logger().log(Severity::Error, msg);
  }

  Process* spawn_plugin(Server& s, std::vector<std::string> args,
                        uv_loop_t* loop)
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
    p->io.in.action_cb = compile_buffer;
    uv_read_start((uv_stream_t*) &p->io, alloc, collect_lines);

    p->err.action_cb = log_buffer;
    uv_read_start((uv_stream_t*) &p->err, alloc, collect_lines);

    // Tell the process to begin.
    // TODO: Inform the child process of Server state here.
    uv_write_t* req = new uv_write_t;

    *p->io.out.buf = vec_from_string("PpM");
    write_buffer(&p->io.out);

    return p;
  }
}
