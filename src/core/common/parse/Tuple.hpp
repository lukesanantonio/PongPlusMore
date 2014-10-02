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
#include <type_traits>
#include <tuple>
#include "../template_utility.hpp"
#include "forward.h"
namespace pong { namespace parse
{
  template <class... Types>
  struct Tuple
  {
    using tuple_t = std::tuple<typename Types::value_t...>;
    static tuple_t parse(Json::Value const&) noexcept;
  };

  template <int N, class... Types>
  std::enable_if_t<N >= sizeof...(Types)>
  parse_element(std::tuple<Types...>&, Json::Value const&) noexcept {}

  template <int N, class... Types>
  std::enable_if_t<N < sizeof...(Types)>
  parse_element(std::tuple<Types...>& tup, Json::Value const& json) noexcept
  {
    // Find the parser required to parse the type in question.
    using active_type = pack_element_t<N, Types...>;
    using parser_t = find_parser_t<active_type>;

    // Actually do the parse.
    std::get<N>(tup) = parser_t::parse(json[N]);
    // And the next one!
    parse_element<N+1, Types...>(tup, json);
  }

  template <class... Types> typename Tuple<Types...>::tuple_t
  Tuple<Types...>::parse(Json::Value const& json) noexcept
  {
    tuple_t tup;
    parse_element<0, tuple_t, Types...>(tup, json);
    return tup;
  }
} }
