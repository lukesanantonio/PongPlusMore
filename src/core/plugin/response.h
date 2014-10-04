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
  template <class Result_Type>
  struct Response
  {
    req_id id;
    Result_Type result;
    bool error = false;
  };
}
BEGIN_FORMATTER_SCOPE
{
  template <class Result_Type>
  DECLARE_TEMPLATE_FORMATTER(pong::Response<Result_Type>);

  template <class Result_Type>
  DEFINE_TEMPLATE_PARSER(json, pong::Response<Result_Type>)
  {
    pong::Response<Result_Type> res;
    res.id = json["id"].asUInt();
    if(json.isMember("result"))
    {
      res.result = json["result"];
      res.error = false;
    }
    else
    {
      res.result = json.get("error", 0);
      res.error = true;
    }
    return res;
  }

  template <class Result_Type>
  DEFINE_TEMPLATE_DUMPER(res, pong::Response<Result_Type>)
  {
    Json::Value val;
    val["id"] = res.id;
    if(!res.error) val["result"] = res.result;
    else val["error"] = res.error;
  }
}
END_FORMATTER_SCOPE
