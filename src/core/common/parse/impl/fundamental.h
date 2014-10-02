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

#define DEFINE_FUNDAMENTAL_PARSER(type, func_suffix, json_method)\
template <> struct parser<type> {\
  inline static type parse(Json::Value const& json) noexcept\
  {\
    return json.json_method();\
  }\
};\
inline type parse_##func_suffix(Json::Value const& json) noexcept\
{\
  return parser<type>::parse(json);\
}

namespace pong
{
  template <typename Type>
  class parser;

  DEFINE_FUNDAMENTAL_PARSER(int, int, asInt);
  DEFINE_FUNDAMENTAL_PARSER(float, float, asFloat);
  DEFINE_FUNDAMENTAL_PARSER(double, double, asDouble);
  DEFINE_FUNDAMENTAL_PARSER(bool, bool, asBool);
  DEFINE_FUNDAMENTAL_PARSER(std::string, string, asString);
}
