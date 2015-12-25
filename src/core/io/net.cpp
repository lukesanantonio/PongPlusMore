/*
 * uGlue - Glue many languages together into a whole with ukernel-inspired RPC.
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
#include "net.h"
#include "common.h"

namespace ug { namespace net
{
  UDP_Handle* create_udp_handle(uv_loop_t* loop) noexcept
  {
    UDP_Handle* self = new UDP_Handle;
    init_udp_handle(*self, loop);
    return self;
  }
  void delete_udp_handle(UDP_Handle* self) noexcept
  {
    uninit_udp_handle(*self);
    delete self;
  }
  void init_udp_handle(UDP_Handle& self, uv_loop_t* loop) noexcept
  {
    self.buf = new buf_t();
    uv_udp_init(loop, &self.handle);
  }
  void uninit_udp_handle(UDP_Handle& self) noexcept
  {
    delete self.buf;
    uv_close((uv_handle_t*) &self.handle, NULL);
  }

  void recieve_udp(uv_udp_t* handle, ssize_t nread, const uv_buf_t* buf,
                   const struct sockaddr* addr, unsigned flags) noexcept
  {
    Pipe* pipe = (Pipe*) handle;

    // If we are given the nothing-to-read notification, flush the buffer.
    if(nread == 0 && addr == nullptr)
    {
      if(pipe->read_cb) pipe->read_cb(pipe);
      pipe->in.buf->clear();
    }

    for(char* cur = buf->base; cur != buf->base + nread; ++cur)
    {
      pipe->in.buf->push_back(*cur);
    }

    // Unallocate the buffer.
    delete[] buf->base;
  }

  Pipe* create_pipe(uv_loop_t* loop, std::string const& bind_ip,
                    uint16_t const port)
  {
    Pipe* self = new Pipe;
    init_pipe(*self, loop, bind_ip, port);
    return self;
  }
  void delete_pipe(Pipe* self) noexcept
  {
    uninit_pipe(*self);
    delete self;
  }
  void init_pipe(Pipe& self, uv_loop_t* loop,
                 std::string const& bind_ip, uint16_t const port)
  {
    self.user_data = nullptr;
    self.read_cb = nullptr;
    self.write_cb = nullptr;

    init_udp_handle(self.in, loop);
    init_udp_handle(self.out, loop);

    // Bind the input socket to accept all connections.
    struct sockaddr_in addr;
    if(uv_ip4_addr(bind_ip.c_str(), port, &addr))
    {
      throw Bind_Error{bind_ip};
    }

    uv_udp_bind(&self.in.handle, (sockaddr*) &addr, 0);
    uv_udp_recv_start(&self.in.handle, alloc, recieve_udp);
  }
  void uninit_pipe(Pipe& self) noexcept
  {
    uv_udp_recv_stop(&self.in.handle);

    uninit_udp_handle(self.in);
    uninit_udp_handle(self.out);
  }

  namespace {
    struct Send_Req
    {
      uv_udp_send_t req;
      uv_buf_t buf;
      Pipe* pipe;
    };
    void after_write(uv_udp_send_t* r, int status)
    {
      Send_Req* req = (Send_Req*) r;
      if(req->pipe->write_cb) req->pipe->write_cb(req->pipe);
      delete req;
    }
  }
  void write_buffer(Pipe& p, sockaddr const* dest) noexcept
  {
    Send_Req* req = new Send_Req;
    req->pipe = &p;

    // Why is this such an issue?
    req->buf.base = reinterpret_cast<char*>(p.out.buf->data());
    req->buf.len = p.out.buf->size();

    uv_udp_send(&req->req, &p.out.handle, &req->buf, 1, dest, after_write);
  }
} }
