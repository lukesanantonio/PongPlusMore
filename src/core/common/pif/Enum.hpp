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
#include "forward.h"
BEGIN_FORMATTER_SCOPE
{
  template <typename Type>
  struct Enum_Desc;

  template <typename Type>
  struct Enum
  {
    static Type parse(Json::Value const&);
    static Json::Value dump(Type const&);
  };

  template <int N, class Enum_Desc>
  std::enable_if_t<N == std::tuple_size<decltype(Enum_Desc::names)>::value - 1,
                   typename Enum_Desc::underlying_t>
  find_enum_value(Json::Value const& json)
  {
    return Enum_Desc::names[N].from;
  }
  template <int N, class Enum_Desc>
  std::enable_if_t<N < std::tuple_size<decltype(Enum_Desc::names)>::value - 1,
                   typename Enum_Desc::underlying_t>
  find_enum_value(Json::Value const& json)
  {
    if(json.asString() == Enum_Desc::names[N].str)
    {
      return Enum_Desc::names[N].from;
    }
    else return find_enum_value<N + 1, Enum_Desc>(json);
  }

  template <typename Type>
  Type Enum<Type>::parse(Json::Value const& json)
  {
    using desc_type = Enum_Desc<Type>;
    using enum_type = typename Enum_Desc<Type>::enum_t;
    return static_cast<enum_type>(find_enum_value<0, desc_type>(json));
  }

  template <int N, class Enum_Desc>
  std::enable_if_t<N == std::tuple_size<decltype(Enum_Desc::names)>::value - 1,
                   Json::Value>
  find_enum_str(typename Enum_Desc::underlying_t const& val)
  {
    return Json::Value(Enum_Desc::names[N].str);
  }

  template <int N, class Enum_Desc>
  std::enable_if_t<N < std::tuple_size<decltype(Enum_Desc::names)>::value - 1,
                   Json::Value>
  find_enum_str(typename Enum_Desc::underlying_t const& val)
  {
    if(Enum_Desc::names[N].from == val)
    {
      return Json::Value(Enum_Desc::names[N].str);
    }
    return find_enum_str<N + 1, Enum_Desc>(val);
  }

  template <typename Type>
  Json::Value Enum<Type>::dump(Type const& val)
  {
    using desc_type = Enum_Desc<Type>;

    using underlying_t = typename desc_type::underlying_t;
    underlying_t val_id = static_cast<underlying_t>(val);

    return find_enum_str<0, desc_type>(val_id);
  }

  template <class Type>
  struct find_formatter<Type, std::enable_if_t<std::is_enum<Type>::value> >
  {
    using type = Enum<Type>;
  };
}
END_FORMATTER_SCOPE
