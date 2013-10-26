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
#include "vector.h"
#include <cmath>
namespace pong
{
  namespace math
  {
    math::vector operator+(math::vector vec1, math::vector vec2)
    {
      return {vec1.x + vec2.x, vec1.y + vec2.y};
    }

    math::vector operator*(double scalar, math::vector vec1)
    {
      return {vec1.x * scalar, vec1.y * scalar};
    }

    math::vector normalize(math::vector vec)
    {
      //Calculate the length
      double length = math::length(vec);

      //We don't want to divide by zero!
      if(length == 0) return vec;

      //Divide each value in the vector by the length!
      vec.x /= length;
      vec.y /= length;

      //Return the new, normalized vector.
      return vec;
    }
    double length(math::vector vec)
    {
      double length = std::sqrt(std::pow(vec.x, 2) + std::pow(vec.y, 2));
      return length;
    }
  }
};
