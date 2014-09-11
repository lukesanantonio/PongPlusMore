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
#include "volume.h"
#include "server/req.h"
#include "server/Logger.h"
#include "server/Object.h"
#include "json/json.h"

#define DECLARE_PARSER(type, func_suffix)\
template <> struct parser<type> {\
  static type parse(Json::Value const&) noexcept;\
};\
inline type parse_##func_suffix(Json::Value const& json) noexcept\
{\
  return parser<type>::parse(json);\
}

namespace pong
{
  template <typename Type>
  class parser;

  DECLARE_PARSER(int, int);
  DECLARE_PARSER(std::string, string);

  DECLARE_PARSER(math::vector<double>, vector);
  DECLARE_PARSER(Volume, volume);
  DECLARE_PARSER(Object, object);
  DECLARE_PARSER(Severity, severity);

  inline std::string to_string(const Json::Value& v) noexcept
  {
    Json::FastWriter w;
    return w.write(v);
  }
}

#undef DECLARE_PARSER
