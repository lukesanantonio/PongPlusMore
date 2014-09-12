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
#include "vector.h"
#include "parse/helper.h"
#include <unordered_map>
#include <array>
namespace pong
{
  using VolumeSides = unsigned short;
  namespace VolumeSide
  {
    constexpr VolumeSides None   = 0x00;
    constexpr VolumeSides Left   = 0x01;
    constexpr VolumeSides Right  = 0x02;
    constexpr VolumeSides Top    = 0x04;
    constexpr VolumeSides Bottom = 0x08;
  }

  /*!
   * \brief Find vector direction using a VolumeSide bitmask representation.
   */
  template <typename point_type>
  inline VolumeSides find_direction(const math::vector<point_type>& v) noexcept
  {
    VolumeSides bit = VolumeSide::None;

    if(v.x < 0) bit |= VolumeSide::Left;
    if(0 < v.x) bit |= VolumeSide::Right;
    if(v.y < 0) bit |= VolumeSide::Top;
    if(0 < v.y) bit |= VolumeSide::Bottom;

    return bit;
  }

  /*!
   * \brief Reduces a vector using passed in constraints.
   *
   * For each side, the component of the vector moving in that direction is
   * zeroed out.
   */
  template <typename point_type>
  inline math::vector<point_type> constrain(math::vector<point_type> v,
                                            VolumeSides constraints) noexcept
  {
    if(constraints & VolumeSide::Left)
      v.x = std::max(v.x, 0.0);
    if(constraints & VolumeSide::Right)
      v.x = std::min(v.x, 0.0);
    if(constraints & VolumeSide::Top)
      v.y = std::max(v.y, 0.0);
    if(constraints & VolumeSide::Bottom)
      v.y = std::min(v.y, 0.0);

    return v;
  }

  /*!
   * \brief Returns a bitmask of volume sides that reflect the opposite sides
   * of the bitmask passed in.
   */
  inline VolumeSides flip(VolumeSides s) noexcept
  {
    VolumeSides mask = VolumeSide::None;
    if(s & VolumeSide::Left) mask |= VolumeSide::Right;
    if(s & VolumeSide::Right) mask |= VolumeSide::Left;
    if(s & VolumeSide::Top) mask |= VolumeSide::Bottom;
    if(s & VolumeSide::Bottom) mask |= VolumeSide::Top;
    return mask;
  }

  struct Volume
  {
    using point_type = double;
    math::vector<point_type> pos;
    point_type width, height;

    Volume(math::vector<point_type> pos = math::vector<point_type>{},
           point_type width = 0, point_type height = 0)
           : pos(pos), width(width), height(height) {}

    DECLARE_PROPERTY_VALUES(3, "Position", "Width", "Height");
    DECLARE_PROPERTIES_TUPLE(math::vector<double>, double, double);
  };

  inline bool operator==(const Volume& v1, const Volume& v2) noexcept
  {
    return v1.pos == v2.pos && v1.width == v2.width && v1.height == v2.height;
  }

  /*!
   * \brief A macro for generating local variables of Volume bounds.
   */
  #define GENERATE_VOLUME_BOUNDS(vol) \
    Volume::point_type vol##_left = vol.pos.x, \
                       vol##_right = vol.pos.x + vol.width - 1, \
                       vol##_top = vol.pos.y, \
                       vol##_bottom = vol.pos.y + vol.height - 1

  /*!
   * \brief Check whether there is an intersection between two volumes.
   */
  bool intersecting(const Volume& v1, const Volume& v2) noexcept;

  /*!
   * \brief Find all sides where obj is extending beyond the bounds of box.
   */
  VolumeSides extending_sides(const Volume& obj, const Volume& box) noexcept;

  /*!
   * \brief Find the side of v1 that is closest to v2.
   */
  VolumeSides closest_side(const Volume& v1, const Volume& v2) noexcept;

  /*!
   * \brief Find whether obj is inside box (inclusively).
   */
  bool inside(const Volume& box, const Volume& obj) noexcept;

  /*!
   * \brief Find the distance of each side of v1 to each side of v2.
   */
  std::unordered_map<VolumeSides, double>
  volume_difference(const Volume& v1, const Volume& v2) noexcept;

  /*!
   * \brief Find the delta required to move v to side s of volume ref.
   */
  math::vector<double>
  outside_snap(const Volume& v, VolumeSides s, const Volume& ref) noexcept;

  /*!
   * \brief Find the delta required to move v to side s of volume ref.
   */
  math::vector<double>
  inside_snap(const Volume& v, VolumeSides s, const Volume& ref) noexcept;

  /*!
   * \brief Find the four quads that make up the volume v.
   */
  std::array<Volume, 4> volume_quads(const Volume& v) noexcept;

  /*!
   * \brief Find the center point of a given volume.
   */
  inline math::vector<double> center(const Volume& v) noexcept
  {
    GENERATE_VOLUME_BOUNDS(v);
    return {v_left + v.width / 2, v_top + v.height / 2};
  }

  /*!
   * \brief Find the normalized vector representing the direction from origin
   * to target.
   */
  inline math::vector<double>
  vector_towards(const Volume& origin, const Volume& target) noexcept
  {
    return math::normalize(center(target) - center(origin));
  }
}
