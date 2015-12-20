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

#include <string>
#include <vector>
#include <deque>
#include <functional>
#include <utility>

#include "core/common/ID_Map.hpp"
#include "core/plugin/req.h"
#include "core/common/log.h"

namespace client
{
  struct Client
  {
    using cb_t = std::function<void(pong::Response const&)>;

    bool poll_request(pong::Request& req) noexcept;

    void post_request(pong::Request const& req) noexcept;
    void post_request(pong::Request const& req, cb_t callback) noexcept;

    void step() noexcept;
  private:
    std::deque<pong::Request> reqs_;
    std::vector<std::pair<pong::req_id_t, cb_t> > cbs_;
  };

  namespace detail
  {
    template <class Type>
    inline void make_params_impl(Json::Value& val, Type const& t)
    {
      val.append(FORMATTER_TYPE(Type)::dump(t));
    }
    template <class Type, class... Types>
    inline void make_params_impl(Json::Value& val,
                                 Type const& t1,
                                 Types&&... params)
    {
      // Append the first one first,
      make_params_impl(val, t1);
      // Then the others.
      make_params_impl(val, std::forward<Types>(params)...);
    }
  }
  template <class... Types>
  inline Json::Value make_params(Types&&... params)
  {
    Json::Value val(Json::ValueType::arrayValue);
    detail::make_params_impl(val, std::forward<Types>(params)...);
    return val;
  }

  template <class T>
  void set_member(Client& c,
                  pong::req_id_t request_id,
                  std::string const& set_method,
                  pong::id_type id,
                  T const& member,
                  Client::cb_t callback) noexcept
  {
    c.post_request({request_id, set_method, make_params(id, member)},
                   callback);
  }

  void log_message(Client&, pong::Log_Severity, std::string const&) noexcept;
}
