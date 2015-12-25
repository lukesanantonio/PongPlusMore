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
#include <memory>
#include <queue>
#include <vector>
#include <boost/variant.hpp>
#include <boost/optional.hpp>
#include <msgpack.hpp>
#include "req.h"
#include "../io/external_io.h"
namespace ug
{
  struct Plugin
  {
    virtual bool poll_request(Request& req) = 0;
    virtual void post_request(Request const& res) noexcept = 0;
  };

  struct Msgpack_Plugin : public Plugin
  {
    Msgpack_Plugin(std::unique_ptr<External_IO> io) noexcept;
    ~Msgpack_Plugin() noexcept = default;

    Msgpack_Plugin(Msgpack_Plugin&&) = default;
    Msgpack_Plugin(Msgpack_Plugin const&) noexcept = delete;

    Msgpack_Plugin& operator=(Msgpack_Plugin&&) = default;
    Msgpack_Plugin& operator=(Msgpack_Plugin const&) noexcept = delete;

    bool poll_request(Request& req) override;
    void post_request(Request const& res) noexcept override;

    inline External_IO& io() noexcept { return *this->io_; }
    inline External_IO const& io() const noexcept { return *this->io_; }
  private:
    std::unique_ptr<External_IO> io_;
    msgpack::unpacker unpacker_;
    std::queue<msgpack::object_handle> raw_reqs_;
  };
}
