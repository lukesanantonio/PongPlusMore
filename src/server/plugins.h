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
#include "common/util.h"
namespace pong
{
  struct Server_Plugin
  {
    virtual bool poll_request(net::req::Request& req) noexcept = 0;
    virtual void post_result(net::req::Request const& req) noexcept = 0;
  };

  struct Json_Plugin : public Server_Plugin
  {
    Json_Plugin(std::unique_ptr<External_IO> io) noexcept;
    ~Json_Plugin() noexcept = default;

    Json_Plugin(Json_Plugin&&) noexcept = default;
    Json_Plugin(Json_Plugin const&) noexcept = delete;

    Json_Plugin& operator=(Json_Plugin&&) noexcept = default;
    Json_Plugin& operator=(Json_Plugin const&) noexcept = delete;

    bool poll_request(net::req::Request& req) noexcept override;
    void post_result(net::req::Request const& req) noexcept override;
  private:
    std::unique_ptr<External_IO> io_;
    std::queue<std::vector<char> > bufs_;
  };
}
