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

#define DECLARE_STRING(str) static constexpr const char* method_name = str
#define BASE_CONSTRUCTION(classname) \
classname(Request_Base const& base) : Request_Base(base) {}

namespace pong { namespace net { namespace req
{
  struct Request_Base
  {
    virtual ~Request_Base() noexcept {};

    id_type id;
  };

  struct Null : public Request_Base
  {
    Null() noexcept = default;
    BASE_CONSTRUCTION(Null);

    DECLARE_STRING("Server.Null");
  };
  struct Log : public Request_Base
  {
    Log() = default;
    BASE_CONSTRUCTION(Log);

    Severity severity;
    std::string msg;

    DECLARE_STRING("Server.Log");
  };
  struct CreateObject : public Request_Base
  {
    CreateObject() = default;
    BASE_CONSTRUCTION(CreateObject);

    Object obj;
    using callback_t = std::function<void (id_type)>;
    callback_t callback;

    DECLARE_STRING("Server.CreateObject");
  };
  struct DeleteObject : public Request_Base
  {
    DeleteObject() noexcept = default;
    BASE_CONSTRUCTION(DeleteObject);

    id_type obj_id;

    DECLARE_STRING("Server.DeleteObject");
  };

  using Request = boost::variant<Null, Log, CreateObject, DeleteObject>;
} } }
