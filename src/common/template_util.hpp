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
 *
 * \file Cache.h
 * \brief Contains the Cache_Impl class declaration (and aliases).
 */
#include <tuple>
#include <type_traits>
namespace pong
{
  namespace detail
  {
    struct no {};

    template <class T>
    no operator==(const T& t1, const T& t2);

    template <class T>
    struct has_equality
    {
      static constexpr bool value =
                !std::is_same<decltype(std::declval<T>() == std::declval<T>()),
                              no>::value;
    };
  }

  template <class T>
  struct has_equality : public
               std::integral_constant<bool, detail::has_equality<T>::value> {};

  template <int N, class F, class... Types, class... Args>
  inline auto call(typename std::enable_if<
                     !(N < std::tuple_size< std::tuple<Types...> >::value), F>
                   ::type /* F */ f,
                   std::tuple<Types...>& t,
                   Args&&... args) noexcept -> decltype(auto)
  {
    return f(std::forward<Args>(args)...);
  }
  template <int N, class F, class... Types, class... Args>
  inline auto call(typename std::enable_if<
                         N < std::tuple_size< std::tuple<Types...> >::value, F>
                   ::type /* F */ f,
                   std::tuple<Types...>& t,
                   Args&&... args) noexcept -> decltype(auto)
  {
    return call<N+1, F>(f, t, std::forward<Args>(args)..., std::get<N>(t));
  }
}
