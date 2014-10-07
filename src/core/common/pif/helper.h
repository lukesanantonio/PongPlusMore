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
#include <tuple>
#include <array>

#define DEFINE_FUNDAMENTAL_FORMATTER(type, json_method)\
template <> struct formatter<type> {\
  inline static type parse(Json::Value const& json) noexcept\
  {\
    return json.json_method();\
  }\
  inline static Json::Value dump(type const& t) noexcept\
  {\
    return Json::Value(t);\
  }\
}

#define FORMATTER_MEMBER_FUNCTIONS(type)\
  static type parse(Json::Value const&) noexcept;\
  static Json::Value dump(type const&) noexcept

#define DEFINE_PARSER(type, vname)\
type formatter<type>::parse(Json::Value const& vname) noexcept

#define DEFINE_DUMPER(type, oname)\
Json::Value formatter<type>::dump(type const& oname) noexcept

#define DECLARE_FORMATTER(type)\
template <> struct formatter<type> {\
  FORMATTER_MEMBER_FUNCTIONS(type);\
}

// User must provide the template<typename, ...> bit.
#define DECLARE_TEMPLATE_FORMATTER(...)\
struct formatter<__VA_ARGS__>\
{\
  FORMATTER_MEMBER_FUNCTIONS(__VA_ARGS__);\
}

#define DEFINE_TEMPLATE_PARSER(vname, ...)\
__VA_ARGS__ formatter<__VA_ARGS__>::parse(Json::Value const& vname) noexcept

#define DEFINE_TEMPLATE_DUMPER(oname, ...)\
Json::Value formatter<__VA_ARGS__>::dump(__VA_ARGS__ const& oname) noexcept

#define DECLARE_PROPERTY_VALUES(size, ...)\
  constexpr static const int property_values_size = size;\
  constexpr static const std::array<const char*, property_values_size>\
  property_values = {__VA_ARGS__}

#define DEFINE_PROPERTY_VALUES(type)\
  constexpr const std::array<const char*, type::property_values_size>\
  type::property_values

#define DEFINE_TEMPLATE_PROPERTY_VALUES(type)\
  template <typename T>\
  constexpr const std::array<const char*, type<T>::property_values_size>\
  type<T>::property_values

#define DECLARE_PROPERTIES_TUPLE(...)\
  using properties_tuple = std::tuple<__VA_ARGS__>

#define PROPERTIES_TUPLE_TYPE properties_tuple

#define DECLARE_PROPERTIES(...)\
  inline PROPERTIES_TUPLE_TYPE properties() const noexcept\
  {\
    return PROPERTIES_TUPLE_TYPE(__VA_ARGS__);\
  }

#include "forward.h"

BEGIN_FORMATTER_SCOPE { namespace detail
{
  template <typename value_t>
  struct Enum_Value
  {
    value_t from;
    char const* const str;
  };
} }
END_FORMATTER_SCOPE

#define BEGIN_DECLARE_FORMATTABLE_ENUM(enum, underlying_type, size)\
template <> struct Enum_Desc<enum>\
{\
  using enum_t = enum;\
  using underlying_t = underlying_type;\
  constexpr static const std::size_t names_size = size;\
  constexpr static const std::array<\
                                detail::Enum_Value<underlying_t>, size> names={

#define ENUM_VALUE(val, str) detail::Enum_Value<underlying_t>{static_cast<underlying_t>(val), str}

#define END_DECLARE_FORMATTABLE_ENUM(enum) }; };\

#define DEFINE_FORMATTABLE_ENUM(enum)\
  constexpr const std::size_t Enum_Desc<enum>::names_size;\
  constexpr const std::array<\
    detail::Enum_Value<typename Enum_Desc<enum>::underlying_t>,\
                       Enum_Desc<enum>::names_size> Enum_Desc<enum>::names;

BEGIN_FORMATTER_SCOPE { namespace detail
{
  // This tag is used internally to mark an object as parsed by the
  // parse::Object template.
  struct as_object {};

  // Likewise, but marks an object as having a custom implementation of the
  // parser template.
  struct as_custom {};
} }
END_FORMATTER_SCOPE

#define DECLARE_FORMATTED_AS_OBJECT\
  using formatter_type = FORMATTER_NAMESPACE::detail::as_object;
#define DECLARE_FORMATTED_WITH_CUSTOM_IMPL\
  using formatter_type = FORMATTER_NAMESPACE::detail::as_custom;

#include "Object.hpp"
#include "Tuple.hpp"
#include "Enum.hpp"

// Now the client code can parse the standard types in it's custom parse impl.
#include "impl/fundamental.h"

BEGIN_FORMATTER_SCOPE
{
  template <class T> struct formatter;

  template <class Type, class Enable>
  struct find_formatter
  {
    using type = formatter<Type>;
  };

  template <class Type>
  struct find_formatter<Type,
           typename pong::enable_if_type<typename Type::formatter_type>::type >
  {
    using type =
      std::conditional_t<std::is_same<detail::as_object,
                                      typename Type::formatter_type>::value,
                         Object<Type>, formatter<Type> >;
  };

  template <class... Types>
  struct find_formatter<std::tuple<Types...>, void>
  {
    using type = Tuple<Types...>;
  };
}
END_FORMATTER_SCOPE
