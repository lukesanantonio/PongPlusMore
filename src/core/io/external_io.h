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
#include <vector>
#include <string>
#include <functional>
#include "ipc.h"
#include "net.h"
namespace pong
{
  struct External_IO
  {
    using read_cb = std::function<void (std::vector<char> const&)>;

    External_IO(read_cb r_cb = nullptr, read_cb e_cb = nullptr) noexcept
                : read_cb_(r_cb), err_cb_(e_cb) {}

    inline void set_read_callback(read_cb cb) noexcept;
    inline void set_error_callback(read_cb cb) noexcept;
    inline void post(std::vector<char> const& buf) noexcept;
    inline void post_error(std::vector<char> const& err) noexcept;

    virtual void write(std::vector<char> const& buf) noexcept = 0;
    virtual void step() noexcept = 0;
  private:
    read_cb read_cb_;
    read_cb err_cb_;
  };

  inline void External_IO::set_read_callback(read_cb cb) noexcept
  {
    read_cb_ = cb;
  }
  inline void External_IO::set_error_callback(read_cb cb) noexcept
  {
    err_cb_ = cb;
  }
  inline void External_IO::post(std::vector<char> const& buf) noexcept
  {
    read_cb_(buf);
  }
  inline void External_IO::post_error(std::vector<char> const& buf) noexcept
  {
    err_cb_(buf);
  }

  struct Child_Process : public External_IO
  {
    Child_Process(ipc::Spawn_Options&) noexcept;
    ~Child_Process() noexcept;

    void write(std::vector<char> const& buf) noexcept override;
    void step() noexcept override;
  private:
    ipc::Process* process_;
    uv_loop_t loop_;
  };

  struct Net_IO : public External_IO
  {
    Net_IO(std::string const& bind_ip, uint16_t const bind_port,
           std::string const& write_ip, uint16_t const write_port) noexcept;
    ~Net_IO() noexcept;

    void write(std::vector<char> const& buf) noexcept override;
    void step() noexcept override;
  private:
    struct sockaddr_in write_addr_;
    net::Pipe pipe_;
    uv_loop_t loop_;
  };
}
