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
#include <vector>
#include "forward.h"
BEGIN_FORMATTER_SCOPE
{
  template <class T, class A>
  struct Vector
  {
    using vector_t = std::vector<T, A>;
    static vector_t parse(Json::Value const&) noexcept;
    static Json::Value dump(vector_t const&) noexcept;
  };

  template <class T, class A>
  typename Vector<T, A>::vector_t
  Vector<T, A>::parse(Json::Value const& json) noexcept
  {
    vector_t vec;
    for(Json::Value const& val : json)
    {
      vec.push_back(FORMATTER_TYPE(T)::parse(val));
    }
    return vec;
  }
  template <class T, class A>
  Json::Value Vector<T, A>::dump(vector_t const& vec) noexcept
  {
    Json::Value json;
    for(T const& t : vec)
    {
      json.append(FORMATTER_TYPE(T)::dump(t));
    }
    return json;
  }

  template <class T, class A>
  struct find_formatter<std::vector<T, A> >
  {
    using type = Vector<T, A>;
  };
}
END_FORMATTER_SCOPE
