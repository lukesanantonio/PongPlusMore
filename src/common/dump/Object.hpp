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
#include "Value.hpp"
namespace pong { namespace dump
{
  template <class Type>
  struct Object
  {
    static Json::Value dump(Type const&) noexcept;
  };

  template <class Type>
  using dumper_t = std::conditional_t<std::is_fundamental<Type>::value,
                                      Value<Type>, Object<Type> >;

  template <class Type, int N, class TupleType>
  std::enable_if_t<N >= std::tuple_size<TupleType>::value>
  populate_json(Json::Value& json, TupleType const& tup) noexcept {}

  template <class Type, int N, class TupleType>
  std::enable_if_t<N < std::tuple_size<TupleType>::value>
  populate_json(Json::Value& json, TupleType const& tup) noexcept
  {
    json[Type::property_values[N]] = std::get<N>(tup);
    populate_json<Type, N+1>(json, tup);
  }

  template <class Type>
  Json::Value Object<Type>::dump(Type const& obj) noexcept
  {
    typename Type::properties_tuple t = obj.properties();
    Json::Value ret;

    populate_json<Type, 0>(ret, t);

    return ret;
  }
} }

