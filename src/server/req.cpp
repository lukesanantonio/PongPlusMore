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
#include "req.h"
namespace pong { namespace net { namespace req
{
  Json::Value Request_Base::response_json() const noexcept
  {
    Json::Value obj(Json::objectValue);
    obj["id"] = this->id;
    if(!this->error_())
    {
      obj["result"] = this->result_();
    }
    else
    {
      obj["error"] = this->result_();
    }

    return obj;
  }

  bool Null::error_() const noexcept { return false; }
  Json::Value Null::result_() const noexcept
  {
    return Json::Value(Json::nullValue);
  }

  bool Log::error_() const noexcept { return !result.success; }
  Json::Value Log::result_() const noexcept
  {
    return Json::Value(result.success);
  }

  bool CreateObject::error_() const noexcept { return !result.obj_id; }
  Json::Value CreateObject::result_() const noexcept
  {
    return Json::Value(result.obj_id);
  }

  bool DeleteObject::error_() const noexcept
  { return !result.success; }
  Json::Value DeleteObject::result_() const noexcept
  {
    return Json::Value(result.success);
  }

  Request_Base const& to_base(Request const& req) noexcept
  {
    struct Base_Visitor : boost::static_visitor<const Request_Base&>
    {
      Request_Base const& operator()(Request_Base const& req) const noexcept
      {
        return req;
      }
    };
    return boost::apply_visitor(Base_Visitor(), req);
  }

  Request create_request(std::string const& method, id_type id) noexcept
  {
    if(method == Log::method_name)
    {
      return Log(id);
    }
    if(method == CreateObject::method_name)
    {
      return CreateObject(id);
    }
    if(method == DeleteObject::method_name)
    {
      return DeleteObject(id);
    }

    return Null(id);
  }
} } }
