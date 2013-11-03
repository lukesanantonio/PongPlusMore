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
/*!
 * \file vector.h
 * \brief The header of a beautiful thing, a vector class.
 */
#pragma once
#include <cmath>
namespace pong
{
  /*!
   * \brief A namespace for all math related pong things.
   *
   * That could be collision detection, vectors, velocities, positions, etc.
   */
  namespace math
  {
    /*!
     * \brief A simple vector class.
     */
    struct vector
    {
      /*!
       * \brief Constructor used for brace initialization with two values,
       * regular construction with two values, etc.
       */
      vector(double x, double y) noexcept : x(x), y(y){}
      /*!
       * \brief The default constructor, basically provides the default points
       * (0, 0) without having to be explicit, used for empty brace
       * initialization and more.
       */
      vector() noexcept : vector(0, 0){}
      /*!
       * \brief X value for anything imaginable.
       */
      double x = 0;
      /*!
       * \brief Y value for anything imaginable.
       */
      double y = 0;
    };

    math::vector operator+(math::vector vec1, math::vector vec2);

    inline math::vector operator-(math::vector vec1)
    {
      return {-vec1.x, -vec1.y};
    }
    inline math::vector operator-(math::vector vec1, math::vector vec2)
    {
      return vec1 + -vec2;
    }

    math::vector operator*(double scalar, math::vector vec1);
    inline math::vector operator*(math::vector vec1, double scalar)
    {
      return scalar * vec1;
    }

    /*!
     * \brief Returns the unit vector of the passed in vector.
     */
    math::vector normalize(math::vector vec);

    /*!
     * \brief Returns the length of the passed in vector.
     */
    double length(math::vector vec);

    inline bool operator==(const vector& left, const vector& right)
    {
      return left.x == right.x && left.y == right.y;
    }
    inline vector truncate(vector vec)
    {
      return {std::trunc(vec.x), std::trunc(vec.y)};
    }
  };
};
