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
#pragma once
#include <uv.h>
#include <vector>
#include <string>
#include <functional>
#include <queue>
#include "../common/maybe_owned.hpp"
#include "ipc.h"
#include "net.h"
namespace ug
{
  /*!
   * \brief Manages and connects client code to some arbitrary io source.
   */
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
    Child_Process(ipc::Spawn_Options&);
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

  struct Pipe_IO : public External_IO
  {
    // Client mode. The counterpart is owned.
    Pipe_IO() noexcept;

    // Get a reference to the counterpart, this function should always return
    // the same reference to the other. Making stuff like:
    // > this->counterpart().counterpart().counterpart().counterpart();
    // work.
    Pipe_IO& counterpart() noexcept;

    // Queue a write to the counterpart.
    void write(std::vector<char> const& buf) noexcept override;

    // Read from the counterpart and send queued buffers.
    void step() noexcept override;
  private:
    void step_(bool recursive) noexcept;

    // Counterpart mode. Counter part is pointed to, not owned.
    Pipe_IO(Pipe_IO& cp) noexcept;

    // Represents a Pipe_IO object that is maybe owned.
    Maybe_Owned<Pipe_IO> cp_;

    std::queue<std::vector<char> > input_;
  };
}
