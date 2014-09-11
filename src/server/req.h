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
  private:
    virtual bool error_() const noexcept = 0;
    virtual Json::Value result_() const noexcept = 0;
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
  };

  using Request = boost::variant<Null, Log, CreateObject, DeleteObject>;

  Request_Base const& to_base(Request const&) noexcept;
  Request create_request(std::string const& method, id_type id) noexcept;
} } }

#undef DECLARE_STRING
#undef BASE_CONSTRUCTION
