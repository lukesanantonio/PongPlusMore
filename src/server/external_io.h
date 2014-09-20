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
#include <functional>
#include "ipc.h"
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

  struct SpawnOptions; struct Logger;
  struct ChildProcess : public External_IO
  {
    ChildProcess(SpawnOptions&, Logger&) noexcept;
    ~ChildProcess() noexcept;

    void write(std::vector<char> const& buf) noexcept override;
    void step() noexcept override;
  private:
    Process* process_;
    uv_loop_t loop_;
  };
}
