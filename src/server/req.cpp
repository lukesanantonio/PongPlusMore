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
#include "common/serialize.h"
#include "common/deserialize.h"
#include "core/io/Logger.h"
#include <stdexcept>
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

  bool Request_Base::parse(Json::Value const& params) noexcept
  {
    try
    {
      this->parse_(params);
    } catch(std::runtime_error& e)
    {
      return false;
    }
    return true;
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
  void Log::parse_(Json::Value const& json)
  {
    this->severity = FORMATTER_TYPE(Severity)::parse(json[0]);
    this->msg = json[1].asString();
  }

  bool CreateObject::error_() const noexcept { return !result.obj_id; }
  Json::Value CreateObject::result_() const noexcept
  {
    return Json::Value(result.obj_id);
  }
  void CreateObject::parse_(Json::Value const& json)
  {
    this->obj = FORMATTER_TYPE(Object)::parse(json[0]);
  }

  bool DeleteObject::error_() const noexcept
  { return !result.success; }
  Json::Value DeleteObject::result_() const noexcept
  {
    return Json::Value(result.success);
  }
  void DeleteObject::parse_(Json::Value const& json)
  {
    this->obj_id = json[0].asInt();
  }

  bool QueryObject::error_() const noexcept { return !this->result.success; }
  Json::Value QueryObject::result_() const noexcept
  {
    if(this->result.success)
    {
      return dump_json(this->result.obj);
    }

    return "Failed to query object with id: " + std::to_string(this->obj_id);
  }
  void QueryObject::parse_(Json::Value const& json)
  {
    this->obj_id = json[0].asInt();
  }

  Json::Value SetObject::result_() const noexcept
  {
    return this->result.success;
  }
  void SetObject::parse_(Json::Value const& json)
  {
    this->obj_id = json[0].asInt();

    const Json::Value& data = json[1];
    if(data.isMember("Volume") && data.isMember("PhysicsOptions"))
    {
      this->data = FORMATTER_TYPE(Object)::parse(data);
    }
    else if(data.isMember("x") && data.isMember("y"))
    {
      this->data = FORMATTER_TYPE(Volume)::parse(data);
    }
    else
    {
      this->data = FORMATTER_TYPE(PhysicsOptions)::parse(data);
    }
  }

  Request_Base const& to_base(Request const& req) noexcept
  {
    struct Base_Visitor : public boost::static_visitor<Request_Base const&>
    {
      Request_Base const& operator()(Request_Base const& req) const noexcept
      {
        return req;
      }
    };
    return boost::apply_visitor(Base_Visitor(), req);
  }
} } }
