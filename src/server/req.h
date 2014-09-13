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
#include "Logger.h"
#include "Object.h"
#include <string>
#include <functional>
#include <boost/variant/variant.hpp>
#include "common/template_util.hpp"
#include "json/json.h"

#define DECLARE_STRING(str) static constexpr const char* method_name = str

namespace pong { namespace net { namespace req
{
  struct Request_Base
  {
    Request_Base(id_type id = 0) : id(id) {}
    virtual ~Request_Base() noexcept {};

    id_type id;

    Json::Value response_json() const noexcept;
    bool parse(Json::Value const&) noexcept;
  private:
    virtual bool error_() const noexcept = 0;
    virtual Json::Value result_() const noexcept = 0;
    virtual void parse_(Json::Value const&) = 0;
  };

  struct Null : public Request_Base
  {
    using Request_Base::Request_Base;
    DECLARE_STRING("Server.Null");

    struct {
    } result;

  private:
    bool error_() const noexcept override;
    Json::Value result_() const noexcept override;
    void parse_(Json::Value const&) override {}
  };
  struct Log : public Request_Base
  {
    using Request_Base::Request_Base;
    DECLARE_STRING("Server.Log");

    Severity severity;
    std::string msg;

    struct {
      bool success;
    } result;

    bool error_() const noexcept override;
    Json::Value result_() const noexcept override;
    void parse_(Json::Value const&) override;
  };
  struct CreateObject : public Request_Base
  {
    using Request_Base::Request_Base;
    DECLARE_STRING("Server.CreateObject");

    Object obj;

    struct {
      id_type obj_id;
    } result;

  private:
    bool error_() const noexcept override;
    Json::Value result_() const noexcept override;
    void parse_(Json::Value const&) override;
  };
  struct DeleteObject : public Request_Base
  {
    using Request_Base::Request_Base;
    DECLARE_STRING("Server.DeleteObject");

    id_type obj_id;

    struct {
      bool success;
    } result;

  private:
    bool error_() const noexcept override;
    Json::Value result_() const noexcept override;
    void parse_(Json::Value const&) override;
  };
  struct QueryObject : public Request_Base
  {
    using Request_Base::Request_Base;
    DECLARE_STRING("Server.QueryObject");

    id_type obj_id;
    struct {
      bool success;
      Object obj;
    } result;

  private:
    bool error_() const noexcept override;
    Json::Value result_() const noexcept override;
    void parse_(Json::Value const&) override;
  };

  using Request_Types = std::tuple<Null, Log, CreateObject, DeleteObject,
                                   QueryObject>;

  using Request = wrap_types<Request_Types, boost::variant>::type;

  Request_Base const& to_base(Request const&) noexcept;

  template <class... Reqs>
  struct Request_Parser
  {
    static boost::variant<Reqs...> parse(Json::Value const&) noexcept;
  };

  template <int N, class... Reqs>
  std::enable_if_t<N >= sizeof...(Reqs), boost::variant<Reqs...> >
  try_parse(Json::Value const& json) noexcept
  {
    // If we haven't found a method match, throw an error up the stack.
    throw std::runtime_error("Undefined method name");
  }

  template <int N, class... Reqs>
  std::enable_if_t<N < sizeof...(Reqs), boost::variant<Reqs...> >
  try_parse(Json::Value const& json) noexcept
  {
    using cur_t = pack_element_t<N, Reqs...>;
    if(cur_t::method_name == json["method"].asString())
    {
      // Construct an object of the request type that matched.
      cur_t req;
      try
      {
        // Set the id.
        req.id = json["id"].asInt();
      }
      catch(std::runtime_error const& e)
      {
        // Good method, but bad id so we can't continue.
        throw std::runtime_error("Failed to parse id: '" +
                                 json["id"].asString() + "'");
      }
      // Parse the json parameters
      if(!req.parse(json["params"]))
      {
        throw std::runtime_error("Invalid parameters");
      }
      return req;
    }
    return try_parse<N+1, Reqs...>(json);
  }

  template <class... Reqs>
  boost::variant<Reqs...>
  Request_Parser<Reqs...>::parse(Json::Value const& json) noexcept
  {
    try
    {
      return try_parse<0, Reqs...>(json);
    }
    catch(std::runtime_error& e)
    {
      // For now if there is an error, log it.
      Log req;
      req.severity = Severity::Error;
      req.msg = e.what();
      return req;
    }
  }
} } }

#undef DECLARE_STRING
