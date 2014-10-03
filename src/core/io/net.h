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
#pragma once
#include <uv.h>
#include <stdexcept>
#include <vector>
#include <string>
#include <cstdint>
namespace pong { namespace net
{
  struct AddrError : public std::runtime_error
  {
    using std::runtime_error::runtime_error;
  };

  struct UDP_Handle
  {
    uv_udp_t handle;
    std::vector<char>* buf;
  };

  UDP_Handle* create_udp_handle(uv_loop_t*) noexcept;
  void delete_udp_handle(UDP_Handle*) noexcept;
  void init_udp_handle(UDP_Handle&, uv_loop_t*) noexcept;
  void uninit_udp_handle(UDP_Handle&) noexcept;

  struct Pipe
  {
    UDP_Handle in;
    UDP_Handle out;

    void* user_data;

    using action_cb = void(*)(Pipe*);
    action_cb read_cb;
    action_cb write_cb;
  };

  Pipe* create_pipe(uv_loop_t*, std::string const& bind_ip,
                    uint16_t const port) noexcept;
  void delete_pipe(Pipe*) noexcept;
  void init_pipe(Pipe&, uv_loop_t*, std::string const& bind_ip,
                 uint16_t const port) noexcept;
  void uninit_pipe(Pipe&) noexcept;

  void write_buffer(Pipe& p, sockaddr const* dest) noexcept;
} }
