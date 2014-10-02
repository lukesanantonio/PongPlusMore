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
#include <tuple>
#include "../parse/helper.h"

#define DECLARE_PROPERTIES(...)\
  inline PROPERTIES_TUPLE_TYPE properties() const noexcept\
  {\
    return PROPERTIES_TUPLE_TYPE(__VA_ARGS__);\
  }

#define DECLARE_DUMPER_TYPE(type) using dumper_type = type

namespace pong { namespace dump
{
  template <class Type> class Object;
  template <class Type> class Value;

  template <class Type>
  using find_dumper_t = std::conditional_t<std::is_fundamental<Type>::value,
                                           Value<Type>,
                                           typename Type::dumper_type >;
} }
