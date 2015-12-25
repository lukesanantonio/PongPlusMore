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
    Params(msgpack::object const& obj,
           std::unique_ptr<msgpack::zone> zone) noexcept;
    Params(msgpack::object_handle&& obj) noexcept;

    Params(Params const& p) noexcept;
    Params(Params&& p) noexcept;

    Params& operator=(Params const& p) noexcept;
    Params& operator=(Params&& p) noexcept;

    msgpack::object_handle object;
  };

  template <std::size_t Where>
  void build_params(msgpack::zone& zone, msgpack::object& p) noexcept
  {
    return;
  }

  template <std::size_t Where, class Arg0, class... ArgN>
  void build_params(msgpack::zone& zone, msgpack::object& p,
                    Arg0&& first_arg, ArgN&&... args) noexcept
  {
    // We assume here that the object already contains an allocated array
    // that has the right size.

    // Set the object wherever we are to that argument.
    p.via.array.ptr[Where] = msgpack::object(first_arg, zone);

    // Then do it again for the next element and the next argument
    build_params<Where + 1>(zone, p, std::forward<ArgN>(args)...);
  }

  template <class... Args>
  Params make_params(Args&&... args) noexcept
  {
    msgpack::zone zone;
    msgpack::object params_obj;

    // This code is based on code from msgpack/adaptor/vector.hpp.
    params_obj.type = msgpack::type::ARRAY;
    params_obj.via.array.ptr = static_cast<msgpack::object*>(
      zone.allocate_align(sizeof(msgpack::object) * sizeof...(Args)));
    params_obj.via.array.size = sizeof...(Args);

    build_params<0>(zone, params_obj, std::forward<Args>(args)...);

    return {params_obj, std::make_unique<msgpack::zone>(std::move(zone))};
  }

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
