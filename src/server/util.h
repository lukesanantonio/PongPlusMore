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
#include <algorithm>
#include "Object.h"
namespace pong
{
  struct NoMorePaddlesAvailable {};
  struct NoMoreBallsAvailable {};
  struct InvalidID {};

  /*!
   * \brief A functor good for std::find and the like. Checks for a specific
   * id.
   */
  struct hasID
  {
    hasID(id_type id = 0) : id(id) {}

    bool operator()(const Object& obj) const noexcept
    {
      return obj.id() == id;
    }

    id_type id;
  };

  /*!
   * \brief Returns the id of an object.
   *
   * The pointer to member function of Object::id isn't sufficient because
   * that function has two overloads which can't be disambiguated without
   * fairly verbose syntax (I believe).
   */
  inline id_type id_of(const Object& obj)
  {
    return obj.id();
  }

  template <class InputIterator>
  inline InputIterator findObjectByID(InputIterator begin, InputIterator end,
                                      id_type id)
  {
    return std::find_if(begin, end, hasID(id));
  }
};
