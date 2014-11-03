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

#include <boost/optional.hpp>
#include <boost/variant.hpp>
#include <string>
#include "json/json.h"

#include "../common/pif/helper.h"

namespace pong
{
  struct null_t {};

  inline bool operator==(null_t n1, null_t n2) noexcept { return true; }

  using id_num_t = int;
  using req_id_t = boost::variant<null_t, id_num_t, std::string>;
  using optional_id_t = boost::optional<req_id_t>;

  bool is_null(req_id_t const& id) noexcept;
  bool is_null(optional_id_t const& id) noexcept;

  struct Invalid_Req_Parse {};
  struct Invalid_Id_Exception : Invalid_Req_Parse {};
  struct Invalid_Request_Exception : Invalid_Req_Parse
  {
    Invalid_Request_Exception() noexcept {}
    explicit Invalid_Request_Exception(optional_id_t id) noexcept
                                       : parsed_id(id) {}
    optional_id_t parsed_id;
  };

  struct Request
  {
    optional_id_t id;
    std::string method;
    boost::optional<Json::Value> params;
  };

  namespace literals
  {
    inline req_id_t operator "" _id(const unsigned long long int id)
    {
      return req_id_t(id);
    }
  }

  inline req_id_t as_id(id_num_t id)
  {
    return req_id_t(id);
  }

  inline bool operator==(Request const& r1, Request const& r2) noexcept
  {
    return r1.id == r2.id && r1.method == r2.method && r1.params == r2.params;
  }
}

BEGIN_FORMATTER_SCOPE
{
  DECLARE_FORMATTER(pong::null_t);
  DECLARE_FORMATTER(pong::req_id_t);
  DECLARE_FORMATTER(pong::Request);
}
END_FORMATTER_SCOPE
