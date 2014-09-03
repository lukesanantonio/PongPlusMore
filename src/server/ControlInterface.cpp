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
  ServerAction compile_command(const std::vector<char>& buf) noexcept
  {
    return ServerAction{};
  }

  struct read_helper_t
  {
    uv_pipe_t handle;
    std::vector<char>* buf;
    Server* s;
  };

  read_helper_t* new_read_obj(Server& s) noexcept
  {
    read_helper_t* t = new read_helper_t;
    std::memset(t, 0, sizeof(read_helper_t));

    t->buf = new std::vector<char>();
    t->s = &s;

    return t;
  }

  void free_read_obj(read_helper_t* t) noexcept
  {
    delete t->buf;
    delete t;
  }

  void enqueue_buffer(read_helper_t* t) noexcept
  {
    // Compile the command.
    ServerAction a = compile_command(*t->buf);
    // Queue the command.
    t->s->enqueue_action(a);
    // Clear the buffer.
    t->buf->clear();
  }

  void collect_commands(uv_stream_t* s, ssize_t nread, const uv_buf_t* buf)
  {
    read_helper_t* req = (read_helper_t*) s;

    if(nread == UV_EOF)
    {
      // Send off the buffer, even if it may not be complete.
      enqueue_buffer(req);
      // Uninitialize and stop reading from stdin.
      uv_read_stop(s);
      uv_close((uv_handle_t*) s, NULL);
      free_read_obj(req);
      return;
    }

    for(char* cur = buf->base; cur != buf->base + nread; ++cur)
    {
      if(*cur == '\n')
      {
        // Enqueue the command, then continue on to the next line.
        enqueue_buffer(req);
        continue;
      }
      req->buf->push_back(*cur);
    }

    // Deallocate the old buffer.
    delete buf->base;
  }

  void alloc(uv_handle_t* handle, size_t ssize, uv_buf_t* buf)
  {
    buf->base = new char[ssize];
    buf->len = ssize;
  }

  void enqueue_events(uv_loop_t* loop, Server& s) noexcept
  {
    read_helper_t* pipe = new_read_obj(s);

    uv_pipe_init(loop, &pipe->handle, 0);
    uv_pipe_open(&pipe->handle, 0);

    uv_read_start((uv_stream_t*)&pipe->handle, alloc, collect_commands);
  }
}
