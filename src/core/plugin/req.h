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

#include <string>
#include <boost/optional.hpp>
#include <msgpack.hpp>

namespace ug
{
  using boost::optional;

  // Functions and ids just happen to have the same type for now.
  using fn_t = uint16_t;
  using id_t = uint16_t;

  // We need to keep the msgpack zone around to keep the memory alive so be it.
  struct Params
  {
    Params() noexcept;
    Params(msgpack::object&& obj, msgpack::zone&& zone) noexcept;

    Params(Params const& p) noexcept;
    Params(Params&& p) noexcept;

    Params& operator=(Params const& p) noexcept;
    Params& operator=(Params&& p) noexcept;

    msgpack::zone zone;
    msgpack::object object;
  };

  struct Request
  {
    // Every request has a function,
    fn_t fn;

    // an optional id to track the response,
    optional<id_t> id;

    // and parameters.
    optional<Params> params;
  };
}
