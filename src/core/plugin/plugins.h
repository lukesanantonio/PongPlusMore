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
#include <boost/variant.hpp>
#include <boost/optional.hpp>
#include "json/json.h"
#include "req.h"
#include "response.h"
#include "../io/external_io.h"
namespace pong
{
  struct Plugin
  {
    virtual bool poll_request(Request& req) = 0;
    virtual void post_response(Response const& res) noexcept = 0;
  };

  struct Json_Parse_Error
  {
    std::string json;
    std::string err;
  };

  // Implements the jsonrpc 2.0 spec. Everything as far as error handling goes
  // and generating the correct responses at that time should be in here.
  struct Json_Plugin : public Plugin
  {
    Json_Plugin(std::unique_ptr<External_IO> io) noexcept;
    ~Json_Plugin() noexcept = default;

    Json_Plugin(Json_Plugin&&) = default;
    Json_Plugin(Json_Plugin const&) noexcept = delete;

    Json_Plugin& operator=(Json_Plugin&&) = default;
    Json_Plugin& operator=(Json_Plugin const&) noexcept = delete;

    bool poll_request(Request& req) override;
    void post_response(Response const& req) noexcept override;

    inline External_IO& io() noexcept;
    inline External_IO const& io() const noexcept;
  private:
    std::unique_ptr<External_IO> io_;
    std::queue<std::vector<char> > bufs_;
  };

  inline External_IO& Json_Plugin::io() noexcept
  {
    return *this->io_;
  }
  inline External_IO const& Json_Plugin::io() const noexcept
  {
    return *this->io_;
  }

  template <class IO_Type, class... Args>
  Json_Plugin make_json_plugin(Args&&... args)
  {
    auto io = std::make_unique<IO_Type>(std::forward<Args>(args)...);
    return Json_Plugin(std::move(io));
  }
}
