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
#include <memory>
#include <queue>
#include <vector>
#include "req.h"
#include "common/deserialize.h"
#include "json/json.h"
#include "external_io.h"
namespace pong
{
  struct Server_Plugin
  {
    virtual bool poll_request(net::req::Request& req) noexcept = 0;
  };

  template <class IO_Type>
  struct Json_Plugin : public Server_Plugin
  {
    template <class... Args>
    Json_Plugin(Args&&...) noexcept;
    Json_Plugin(Json_Plugin&&) noexcept = default;
    Json_Plugin(Json_Plugin const&) noexcept = default;
    ~Json_Plugin() noexcept = default;

    Json_Plugin& operator=(Json_Plugin&&) noexcept = default;
    Json_Plugin& operator=(Json_Plugin const&) noexcept = default;

    bool poll_request(net::req::Request& req) noexcept override;
  private:
    std::unique_ptr<External_IO> io_;
    std::queue<std::vector<char> > bufs_;
  };

  template <class IO_Type>
  template <class... Args>
  Json_Plugin<IO_Type>::Json_Plugin(Args&&... args) noexcept
                  : io_(std::make_unique<IO_Type>(std::forward<Args>(args)...))
  {
    io_->set_read_callback([this](const std::vector<char>& buf)
    {
      bufs_.push(buf);
    });
  }
  template <class IO_Type>
  bool Json_Plugin<IO_Type>::poll_request(net::req::Request& req) noexcept
  {
    io_->step();
    if(!bufs_.size()) return false;

    Json::Value val;

    Json::Reader read(Json::Features::strictMode());

    using std::begin; using std::end;
    // While the queued buffer is failing to compile.
    while(!read.parse(std::string(begin(bufs_.front()), end(bufs_.front())),
                      val))
    {
      // Ignore it and continue on.
      bufs_.pop();
      if(!bufs_.size()) return false;
    }

    req = parse_request(val);
    return true;
  }
}
