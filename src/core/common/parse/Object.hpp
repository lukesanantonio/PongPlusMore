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
#include "helper.hpp"
namespace pong { namespace parse
{
  template <class Type>
  struct Object
  {
    using value_t = Type;
    static Type parse(Json::Value const& root) noexcept;
  };

  template <class Type>
  using prop_tuple_t = typename Type::properties_tuple;

  template <class Type, int N>
  std::enable_if_t<N >= std::tuple_size<prop_tuple_t<Type> >::value >
  parse_tuple_element(Json::Value const& json,
                      typename Type::properties_tuple& tup) noexcept {}

  template <class Type, int N>
  std::enable_if_t<N<std::tuple_size<prop_tuple_t<Type> >::value >
  parse_tuple_element(Json::Value const& json,
                      typename Type::properties_tuple& tup)
  {
    // Get the correct parser to use in parsing the current type in question.
    using active_type = std::tuple_element_t<N, prop_tuple_t<Type> >;
    using parser_t = find_parser_t<active_type>;

    // Actually parse the current value of the current type.
    std::get<N>(tup) = parser_t::parse(json[Type::property_values[N]]);
    // And the next one too!
    parse_tuple_element<Type, N+1>(json, tup);
  }

  template <class Type, int N, class TupleType, class... Args>
  std::enable_if_t<N >= std::tuple_size<TupleType>::value, Type>
  construct_from_tuple(TupleType&, Args&&... args) noexcept
  {
    return Type(std::forward<Args>(args)...);
  }

  template <class Type, int N, class TupleType, class... Args>
  std::enable_if_t<N < std::tuple_size<TupleType>::value, Type>
  construct_from_tuple(TupleType& tup, Args&&... args) noexcept
  {
    return construct_from_tuple<Type, N+1>(tup, std::forward<Args>(args)...,
                                           std::get<N>(tup));
  }

  template <class Type>
  Type Object<Type>::parse(Json::Value const& root) noexcept
  {
    // Make a tuple to the store the parsed elements.
    typename Type::properties_tuple tup;

    // Fill the tuple.
    parse_tuple_element<Type, 0>(root, tup);

    // Construct the type in question from the values in the tuple.
    return construct_from_tuple<Type, 0>(tup);
  }
} }
