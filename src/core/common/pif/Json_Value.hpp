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
#include "json/json.h"
#include "forward.h"
BEGIN_FORMATTER_SCOPE
{
  struct Json_Value
  {
    static inline Json::Value dump(Json::Value const&) noexcept;
    static inline Json::Value parse(Json::Value const&) noexcept;
  };
  inline Json::Value Json_Value::dump(Json::Value const& json) noexcept
  {
    return json;
  }
  inline Json::Value Json_Value::parse(Json::Value const& json) noexcept
  {
    return json;
  }
}
END_FORMATTER_SCOPE
