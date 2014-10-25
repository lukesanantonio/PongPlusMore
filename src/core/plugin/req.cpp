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
namespace pong
{
  struct Json_Id_Visitor : public boost::static_visitor<Json::Value>
  {
    template <typename Type>
    Json::Value operator()(Type const& t) const noexcept
    {
      return FORMATTER_TYPE(Type)::dump(t);
    }
  };
}
BEGIN_FORMATTER_SCOPE
{
  DEFINE_PARSER(pong::null_t, json)
  {
    return pong::null_t{};
  }
  DEFINE_DUMPER(pong::null_t, n)
  {
    return Json::ValueType::nullValue;
  }

  DEFINE_PARSER(pong::req_id_t, json)
  {
    pong::req_id_t id;

    if(json.isString())
    {
      id = json.asString();
    }
    else if(json.isIntegral())
    {
      id = json.asInt();
    }
    else if(json.isNull())
    {
      id = pong::null_t{};
    }
    else
    {
      throw pong::Invalid_Id_Exception{};
    }

    return id;
  }
  DEFINE_DUMPER(pong::req_id_t, id)
  {
    Json::Value val = boost::apply_visitor(pong::Json_Id_Visitor(), id);
    return val;
  }
  DEFINE_PARSER(pong::Request, js)
  {
    pong::Request req;

    try
    {
      req.id = FORMATTER_TYPE(pong::req_id_t)::parse(js["id"]);
    }
    catch(pong::Invalid_Id_Exception& e)
    {
      // This means there is a bad id value, maybe it's floating point for
      // instance.
      throw pong::Invalid_Request_Exception{};
    }
    // This means an id wasn't provided at all, which we can ignore.
    catch(std::runtime_error& e) {}

    // We need a method!
    if(!js.get("method", 0).isString())
    {
      throw pong::Invalid_Request_Exception{req.id};
    }
    req.method = js["method"].asString();

    if(js.isMember("params"))
    {
      req.params = js["params"];
    }

    return req;
  }
  DEFINE_DUMPER(pong::Request, req)
  {
    Json::Value json;

    // Only output an id if there is one.
    if(req.id)
    {
      json["id"] = FORMATTER_TYPE(pong::req_id_t)::dump(req.id.value());
    }

    json["method"] = req.method;

    if(req.params)
    {
      json["params"] = req.params.value();
    }

    return json;
  }
}
END_FORMATTER_SCOPE
