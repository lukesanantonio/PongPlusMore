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
 */
/*!
 * \file vector.h
 * \brief The header of a beautiful thing, a vector class.
 */
#pragma once
#include <type_traits>
#include <cstdint>
#include <cmath>
#include <iostream>
#include "pif/helper.h"
namespace pong
{
  /*!
   * \brief Return whether or not check is inside (inclusively) a range.
   *
   * \param left One end of the range.
   * \param right Another end of the range.
   * \param check Number to check.
   *
   * \returns True if check is between left..right or right..left
   * inclusive.
   */
  template <typename P1, typename P2, typename P3>
  constexpr inline bool is_in(P1 left, P2 right, P3 check) noexcept
  {
    return std::min(left, right) <= check && check <= std::max(left, right);
  }

  /*!
   * \brief A simple vector class.
   */
  template <typename point_type>
  struct Vec
  {
    /*!
     * \brief Constructor used for brace initialization with two values,
     * regular construction with two values, etc.
     */
    Vec(point_type x, point_type y) noexcept : x(x), y(y){}
    /*!
     * \brief The default constructor, basically provides the default points
     * (0, 0) without having to be explicit, used for empty brace
     * initialization and more.
     *
     * This function makes constructing a vector from points an all or
     * nothing deal. To construct a vector with its x value makes no sense.
     */
    Vec() noexcept : Vec(0, 0){}

    /*!
     * \brief Any-Compatible-Type conversion/copy constructor.
     */
    template <typename pt2>
    Vec(const Vec<pt2>& vec) : Vec(vec.x, vec.y) {};

    /*!
     * \brief X value for anything imaginable.
     */
    point_type x = 0;
    /*!
     * \brief Y value for anything imaginable.
     */
    point_type y = 0;

    DECLARE_FORMATTED_AS_OBJECT;

    DECLARE_PROPERTY_VALUES(2, "x", "y");
    DECLARE_PROPERTIES_TUPLE(point_type, point_type);
    DECLARE_PROPERTIES(this->x, this->y);
  };

  DEFINE_TEMPLATE_PROPERTY_VALUES(Vec);

  template <typename point_type1, typename point_type2>
  inline bool operator==(const Vec<point_type1>& lhs,
                         const Vec<point_type2>& rhs) noexcept
  {
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
  }
  template <typename point_type1, typename point_type2>
  inline bool operator!=(const Vec<point_type1>& lhs,
                         const Vec<point_type2>& rhs) noexcept
  {
    return !(lhs == rhs);
  }

  template <typename point_type1, typename point_type2>
  inline auto operator+(const Vec<point_type1>& lhs,
                        const Vec<point_type2>& rhs) noexcept
                                           -> Vec<decltype(lhs.x + rhs.x)>
  {
    using point_type = decltype(lhs.x + rhs.x);
    return {static_cast<point_type>(lhs.x + rhs.x),
            static_cast<point_type>(lhs.y + rhs.y)};
  }

  template <typename point_type1, typename point_type2>
  inline Vec<point_type1>
  operator+= (Vec<point_type1>& lhs,
              const Vec<point_type2>& rhs) noexcept
  {
    lhs = lhs + rhs;
    return lhs;
  }

  template <typename point_type>
  inline Vec<point_type> operator-(const Vec<point_type>& vec) noexcept
  {
    return {static_cast<point_type>(-vec.x),
            static_cast<point_type>(-vec.y)};
  }
  template <typename point_type1, typename point_type2>
  inline auto operator-(const Vec<point_type1>& lhs,
                        const Vec<point_type2>& rhs) noexcept
                                           -> Vec<decltype(lhs.x - rhs.x)>
  {
    // lhs.x - rhs.x is *not* the same as lhs.x + -rhs.x if rhs.x is
    // unsigned for some ridiculous reason.
    using point_type = decltype(lhs.x - rhs.x);
    return {static_cast<point_type>(lhs.x - rhs.x),
            static_cast<point_type>(lhs.y - rhs.y)};
  }

  template <typename point_type1, typename point_type2>
  inline decltype(auto) operator-=(Vec<point_type1>& lhs,
                                   const Vec<point_type2>& rhs) noexcept
  {
    return lhs = lhs - rhs;
  }

  template <typename point_type>
  inline Vec<point_type> operator*(const Vec<point_type>& lhs,
                                      double scalar) noexcept
  {
    return {static_cast<point_type>(lhs.x * scalar),
            static_cast<point_type>(lhs.y * scalar)};
  }

  template <typename point_type>
  inline Vec<point_type>
  operator*(double scalar,
            const Vec<point_type>& rhs) noexcept
  {
    return rhs * scalar;
  }

  /*!
   * \brief Returns the unit vector of the passed in vector.
   */
  template <typename point_type,
            class = typename std::enable_if<
                            std::is_floating_point<point_type>::value>::type>
  Vec<point_type> normalize(Vec<point_type> vec)
  {
    //Calculate the length
    point_type len = length(vec);

    //We don't want to divide by zero!
    if(len == 0) return vec;

    //Divide each value in the vector by the length!
    vec.x /= len;
    vec.y /= len;

    //Return the new, normalized vector.
    return vec;
  }

  /*!
   * \brief Returns the length of the passed in vector.
   */
  template <typename point_type,
            class = typename std::enable_if<
                            std::is_floating_point<point_type>::value>::type>
  point_type length(const Vec<point_type>& vec)
  {
    point_type length = std::sqrt(std::pow(vec.x, 2) + std::pow(vec.y, 2));
    return length;
  }

  template <typename point_type>
  inline Vec<point_type> truncate(const Vec<point_type>& vec) noexcept
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

  template <typename P1>
  constexpr decltype(auto) to_radians(P1 degrees) noexcept
  {
    return degrees * M_PI / 180.0;
  }

  /*!
   * \brief Rotates the vector passed in then returns the result.
   *
   * \param rotation The amount to rotate in radians.
   * The rotations go clockwise.
   */
  template <typename point_type, typename num_type>
  inline Vec<point_type> rotate(const Vec<point_type>& vec,
                                   num_type rotation)
  {
    Vec<point_type> rot_vec;

    rot_vec.x = do_round<point_type>(vec.x * std::cos(rotation) -
                vec.y * std::sin(rotation));
    rot_vec.y = do_round<point_type>(vec.x * std::sin(rotation) +
                vec.y * std::cos(rotation));

    return rot_vec;
  }

  template <typename point_type>
  std::ostream& operator<<(std::ostream& out, const Vec<point_type>& vec)
  {
    out << "{" << vec.x << "," << vec.y << "}";
    return out;
  }
}
