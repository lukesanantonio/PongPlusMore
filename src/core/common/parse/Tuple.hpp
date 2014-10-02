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
#include "common/template_util.hpp"
namespace pong { namespace parse
{
  template <class... Types>
  struct Tuple
  {
    using tuple_t = std::tuple<typename Types::value_t...>;
    using value_t = tuple_t;
    static tuple_t parse(Json::Value const&) noexcept;
  };

  template <int N, class Tuple_Type, class... Parsers>
  std::enable_if_t<N >= sizeof...(Parsers)>
  parse_element(Tuple_Type&, Json::Value const&) noexcept {}

  template <int N, class Tuple_Type, class... Parsers>
  std::enable_if_t<N < sizeof...(Parsers)>
  parse_element(Tuple_Type& tup, Json::Value const& json) noexcept
  {
    std::get<N>(tup) = pack_element_t<N, Parsers...>::parse(json[N]);
    parse_element<N+1, Tuple_Type, Parsers...>(tup, json);
  }

  template <class... Types> typename Tuple<Types...>::tuple_t
  Tuple<Types...>::parse(Json::Value const& json) noexcept
  {
    tuple_t tup;
    parse_element<0, tuple_t, Types...>(tup, json);
    return tup;
  }
} }
