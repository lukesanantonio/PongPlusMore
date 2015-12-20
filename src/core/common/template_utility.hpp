/*
 * uGlue - Glue many languages together into a whole with ukernel-inspired RPC.
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
#pragma once
#include <tuple>
#include <type_traits>
namespace ug
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

  template <int N, typename... Params>
  struct pack_element
  {
    using type = std::tuple_element_t<N, std::tuple<Params...> >;
  };

  template <int N, typename... Params>
  using pack_element_t = typename pack_element<N, Params...>::type;

  template <int N, class F, class Tuple_Type, class... Args>
  inline auto
  call_impl(std::enable_if_t<N == std::tuple_size<Tuple_Type>::value, F> f,
            Tuple_Type& tup, Args&&... args) -> decltype(auto)
  {
    return f(std::forward<Args>(args)...);
  }

  template <int N, class F, class Tuple_Type, class... Args>
  inline auto
  call_impl(std::enable_if_t<N < std::tuple_size<Tuple_Type>::value, F> f,
            Tuple_Type& tup, Args&&... args) -> decltype(auto)
  {
    return
    call_impl<N + 1, F, Tuple_Type, Args...>(f, tup,
                                             std::forward<Args>(args)...,
                                             std::get<N>(tup));
  }


  template <class F, class Tuple_Type, class... Args>
  inline auto call(F f, Tuple_Type& tup, Args&&... args) ->decltype(auto)
  {
    return call_impl<0, F, Tuple_Type>(f, tup, std::forward<Args>(args)...);
  }

  template <class, template <class...> class>
  struct wrap_types {};

  template <template <class...> class Wrap, template <class...> class New,
            class... Types>
  struct wrap_types<Wrap<Types...>, New> { using type = New<Types...>; };

  template <class Wrapper, template <class...> class New>
  using wrap_types_t = typename wrap_types<Wrapper, New>::type;

  template <int N, class F, class TupleType, class... Args>
  inline std::enable_if_t<N >= std::tuple_size<TupleType>::value >
  call_foreach(F f, TupleType&& tup, Args&&... args) {}

  template <int N, class F, class TupleType, class... Args>
  inline std::enable_if_t<N < std::tuple_size<TupleType>::value >
  call_foreach(F f, TupleType&& tup, Args&&... args)
  {
    f(std::forward<Args>(args)..., std::get<N>(tup));
    call_foreach<N+1>(f, std::forward<TupleType>(tup),
                      std::forward<Args>(args)...);
  }

  template <class T, class R = void>
  struct enable_if_type
  {
    using type = R;
  };

  namespace detail
  {
    template <typename first, typename... others>
    constexpr bool all_of(first const& f, others const&... o) noexcept
    {
      return f && all_of(o...);
    }
  }

  template <bool... vals>
  struct all_of
    : std::integral_constant<bool, detail::all_of(vals...) > {};
}
