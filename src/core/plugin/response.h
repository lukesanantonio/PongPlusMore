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
#include "req.h"
namespace pong
{
  struct Invalid_Response_Exception {};

  struct Error_Response
  {
    Error_Response(int code = 0, std::string const& msg = "") noexcept
                   : code(code), msg(msg) {}

    int code;
    std::string msg;

    DECLARE_PROPERTY_VALUES(2, "code", "message");
    DECLARE_PROPERTIES_TUPLE(int, std::string);
    DECLARE_PROPERTIES(code, msg);
    DECLARE_FORMATTED_AS_OBJECT;
  };

  using response_result = boost::variant<Json::Value, Error_Response>;

  struct Response
  {
    optional_id_t id;
    response_result result;
  };

  inline bool is_error_response(Response const& res) noexcept
  {
    struct Is_Error_Response_Visitor : public boost::static_visitor<bool>
    {
      bool operator()(Json::Value const& val) const noexcept
      {
        return false;
      }
      bool operator()(Error_Response const& res) const noexcept
      {
        return true;
      }
    };

    return boost::apply_visitor(Is_Error_Response_Visitor(), res.result);
  }

  struct Not_Error_Response_Exception {};

  inline Error_Response const& get_error_response(Response const& res)
  {
    try
    {
      return boost::get<Error_Response>(res.result);
    }
    catch(boost::bad_get& e)
    {
      throw Not_Error_Response_Exception{};
    }
  }
}
BEGIN_FORMATTER_SCOPE
{
  DECLARE_FORMATTER(pong::Response);
}
END_FORMATTER_SCOPE
