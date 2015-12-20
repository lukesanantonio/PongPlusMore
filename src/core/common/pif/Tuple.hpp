/*
 * PpM - ug Plus More - A ug clone full of surprises written with C++11.
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
#include <type_traits>
#include <tuple>
#include "../utility.h"
#include "forward.h"
BEGIN_FORMATTER_SCOPE
{
  template <class... Types>
  struct Tuple
  {
    using tuple_t = std::tuple<Types...>;
    static tuple_t parse(Json::Value const&);
    static Json::Value dump(tuple_t const&);
  };

  // Parsing code.
  template <int N, class... Types>
  std::enable_if_t<N >= sizeof...(Types)>
  parse_element(std::tuple<Types...>&, Json::Value const&) {}

  template <int N, class... Types>
  std::enable_if_t<N < sizeof...(Types)>
  parse_element(std::tuple<Types...>& tup, Json::Value const& json)
  {
    // Find the parser required to parse the type in question.
    using active_t = ug::pack_element_t<N, Types...>;
    using formatter_t = find_formatter_t<active_t>;

    // Actually do the parse.
    std::get<N>(tup) = formatter_t::parse(json[N]);
    // And the next one!
    parse_element<N+1, Types...>(tup, json);
  }

  template <class... Types> typename Tuple<Types...>::tuple_t
  Tuple<Types...>::parse(Json::Value const& json)
  {
    tuple_t tup;
    parse_element<0, Types...>(tup, json);
    return tup;
  }

  // Dumping code.
  template <int N, class Tuple_Type>
  std::enable_if_t<(N == std::tuple_size<Tuple_Type>::value &&
                    std::tuple_size<Tuple_Type>::value > 0)>
  dump_tuple_element(Json::Value&, Tuple_Type const&) {}

  template <int N, class Tuple_Type>
  std::enable_if_t<N == 0 && std::tuple_size<Tuple_Type>::value == 0>
  dump_tuple_element(Json::Value& json, Tuple_Type const&)
  {
    json = Json::Value(Json::ValueType::arrayValue);
  }

  template <int N, class Tuple_Type>
  std::enable_if_t<(N < std::tuple_size<Tuple_Type>::value &&
                   std::tuple_size<Tuple_Type>::value > 0)>
  dump_tuple_element(Json::Value& json, Tuple_Type const& tup)
  {
    // Dump the value of the active type.
    using active_t = std::tuple_element_t<N, Tuple_Type>;
    using formatter_t = find_formatter_t<active_t>;

    // Actually do the dump.
    json[N] = formatter_t::dump(std::get<N>(tup));
    // And the next one!
    dump_tuple_element<N+1>(json, tup);
  }

  template <class... Types> Json::Value
  Tuple<Types...>::dump(typename Tuple<Types...>::tuple_t const& tup)
  {
    // Make our json.
    Json::Value json;
    dump_tuple_element<0>(json, tup);
    return json;
  }
}
END_FORMATTER_SCOPE
