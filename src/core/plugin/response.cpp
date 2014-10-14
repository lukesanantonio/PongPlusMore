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
#include "response.h"
namespace pong
{
  struct Json_Result_Visitor : public boost::static_visitor<Json::Value>
  {
    template <typename Type>
    Json::Value operator()(Type const& t) const noexcept
    {
      return FORMATTER_TYPE(Type)::dump(t);
    }
  };

  DEFINE_PROPERTY_VALUES(pong::Error_Response);
}
BEGIN_FORMATTER_SCOPE
{
  DEFINE_PARSER(pong::Response, json)
  {
    pong::Response res;
    // If there is an id.
    if(static_cast<bool>(res.id))
    {
      res.id = FORMATTER_TYPE(pong::req_id_t)::parse(json["id"]);
    }
    if(json.isMember("result"))
    {
      res.result = json["result"];
    }
    else if(json.isMember("error"))
    {
      res.result = FORMATTER_TYPE(pong::Error_Response)::parse(json["error"]);
    }
    else
    {
      throw pong::Invalid_Response_Exception{};
    }

    return res;
  }

  DEFINE_DUMPER(pong::Response, res)
  {
    Json::Value val;

    if(static_cast<bool>(res.id))
    {
      val["id"] = FORMATTER_TYPE(pong::req_id_t)::dump(res.id.value());
    }

    std::string key;
    if(is_error_response(res))
    {
      key = "error";
    }
    else
    {
      key = "result";
    }
    val[key] = boost::apply_visitor(pong::Json_Result_Visitor(), res.result);

    return val;
  }
}
END_FORMATTER_SCOPE
