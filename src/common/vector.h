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
#include <type_traits>
#include <cstdint>
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
    template <typename point_type = int16_t>
    struct vector
    {
      /*!
       * \brief Constructor used for brace initialization with two values,
       * regular construction with two values, etc.
       */
      vector(point_type x, point_type y) noexcept : x(x), y(y){}
      /*!
       * \brief The default constructor, basically provides the default points
       * (0, 0) without having to be explicit, used for empty brace
       * initialization and more.
       *
       * This function makes constructing a vector from points an all or
       * nothing deal. To construct a vector with its x value makes no sense.
       */
      vector() noexcept : vector(0, 0){}

      /*!
       * \brief Any-Compatible-Type conversion/copy constructor.
       */
      template <typename pt2>
      vector(const vector<pt2>& vec) : vector(vec.x, vec.y) {};

      /*!
       * \brief X value for anything imaginable.
       */
      point_type x = 0;
      /*!
       * \brief Y value for anything imaginable.
       */
      point_type y = 0;
    };

    template <typename point_type1, typename point_type2>
    inline bool operator==(const vector<point_type1>& lhs,
                           const vector<point_type2>& rhs)
    {
      return (lhs.x == rhs.x) && (lhs.y == rhs.y);
    }
    template <typename point_type>
    inline vector<point_type> operator+(const vector<point_type>& lhs,
                                        const vector<point_type>& rhs)
    {
      return {static_cast<point_type>(lhs.x + rhs.x),
              static_cast<point_type>(lhs.y + rhs.y)};
    }

    template <typename point_type>
    inline vector<point_type> operator-(const vector<point_type>& vec)
    {
      return {static_cast<point_type>(-vec.x),
              static_cast<point_type>(-vec.y)};
    }
    template <typename point_type>
    inline vector<point_type> operator-(const vector<point_type>& lhs,
                                        const vector<point_type>& rhs)
    {
      return lhs + -rhs;
    }

    template <typename point_type>
    inline vector<point_type> operator*(const vector<point_type>& lhs,
                                        double scalar)
    {
      return {static_cast<point_type>(lhs.x * scalar),
              static_cast<point_type>(lhs.y * scalar)};
    }

    template <typename point_type>
    inline vector<point_type> operator*(double scalar,
                                        const vector<point_type>& rhs)
    {
      return rhs * scalar;
    }

    /*!
     * \brief Returns the unit vector of the passed in vector.
     */
    template <typename point_type,
              class = typename std::enable_if<
                              std::is_floating_point<point_type>::value>::type>
    vector<point_type> normalize(vector<point_type> vec)
    {
      //Calculate the length
      point_type length = length(vec);

      //We don't want to divide by zero!
      if(length == 0) return vec;

      //Divide each value in the vector by the length!
      vec.x /= length;
      vec.y /= length;

      //Return the new, normalized vector.
      return vec;
    }

    /*!
     * \brief Returns the length of the passed in vector.
     */
    template <typename point_type,
              class = typename std::enable_if<
                              std::is_floating_point<point_type>::value>::type>
    point_type length(const vector<point_type>& vec)
    {
      point_type length = std::sqrt(std::pow(vec.x, 2) + std::pow(vec.y, 2));
      return length;
    }

    template <typename point_type>
    inline bool operator==(const vector<point_type>& lhs,
                           const vector<point_type>& rhs)
    {
      return lhs.x == rhs.x && lhs.y == rhs.y;
    }
    template <typename point_type>
    inline vector<point_type> truncate(const vector<point_type>& vec)
    {
      return {std::trunc(vec.x), std::trunc(vec.y)};
    }

    /*!
     * \brief Rounds the passed in floating point number.
     *
     * This function just rounds the number if it wants to integral, rather
     * than truncation.
     */
    template <typename return_type, typename floating_type>
    static inline typename std::enable_if<std::is_integral<return_type>::value,
                                          return_type>::type
    do_round(floating_type value)
    {
      return static_cast<return_type>(std::round(value));
    }

    /*!
     * \brief Pass through. See other overload.
     */
    template <typename return_type, typename floating_type>
    static inline typename std::enable_if<
                                         !std::is_integral<return_type>::value,
                                         return_type>::type
    do_round(floating_type value)
    {
      return value;
    }

    /*!
     * \brief Rotates the vector passed in then returns the result.
     *
     * \param rotation_degrees The amount of degrees to rotate in radians.
     * The rotations go clockwise.
     */
    template <typename point_type, typename num_type>
    inline vector<point_type> rotate(const vector<point_type>& vec,
                                     num_type rotation_degrees)
    {
      vector<point_type> rot_vec;

      rot_vec.x = do_round<point_type>(vec.x * std::cos(rotation_degrees) -
                  vec.y * std::sin(rotation_degrees));
      rot_vec.y = do_round<point_type>(vec.x * std::sin(rotation_degrees) +
                  vec.y * std::cos(rotation_degrees));

      return rot_vec;
    }
  };
};
