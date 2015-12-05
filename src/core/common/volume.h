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
#pragma once
#include <algorithm>
#include <unordered_map>
#include <array>
#include "vector.h"
#include "pif/helper.h"
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
  inline VolumeSides find_direction(const Vec<point_type>& v) noexcept
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
  inline Vec<point_type> constrain(Vec<point_type> v,
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

  template <typename T>
  struct Volume
  {
    using point_type = T;
    Vec<point_type> pos;
    point_type width, height;

    Volume(Vec<point_type> pos = Vec<point_type>{},
           point_type width = 0, point_type height = 0) noexcept
           : pos(pos), width(width), height(height) {}

    template <typename T2>
    Volume(Volume<T2> const& vol) noexcept
           : pos(vol.pos), width(vol.width), height(vol.height) {}

    DECLARE_FORMATTED_AS_OBJECT;

    DECLARE_PROPERTY_VALUES(3, "Position", "Width", "Height");
    DECLARE_PROPERTIES_TUPLE(Vec<point_type>, point_type, point_type);
    DECLARE_PROPERTIES(this->pos, this->width, this->height);
  };

  DEFINE_TEMPLATE_PROPERTY_VALUES(Volume);

  template <typename T1, typename T2>
  inline bool operator==(const Volume<T1>& v1, const Volume<T2>& v2) noexcept
  {
    return v1.pos == v2.pos && v1.width == v2.width && v1.height == v2.height;
  }

  /*!
   * \brief A macro for generating local variables of Volume bounds.
   */
  #define GENERATE_VOLUME_BOUNDS(vol) \
    typename std::remove_reference_t<decltype(vol)>::point_type\
      vol##_left = vol.pos.x, \
      vol##_right = vol.pos.x + vol.width - 1, \
      vol##_top = vol.pos.y, \
      vol##_bottom = vol.pos.y + vol.height - 1

  /*!
   * \brief Check whether there is an intersection between two volumes.
   *
   * \returns True if the two volumes intersect in any way, false otherwise.
   */
  template <typename T1, typename T2>
  bool intersecting(const Volume<T1>& v1, const Volume<T2>& v2) noexcept
  {
    GENERATE_VOLUME_BOUNDS(v1);
    GENERATE_VOLUME_BOUNDS(v2);

    if(is_in(v1_left, v1_right, v2_left) or
       is_in(v1_left, v1_right, v2_right) or
       (v2_left <= v1_left && v1_right <= v2_right))
    {
      if(is_in(v1_top, v1_bottom, v2_top) or
         is_in(v1_top, v1_bottom, v2_bottom) or
         (v2_top <= v1_top && v1_bottom <= v2_bottom)) return true;
    }

    return false;
  }

  /*!
   * \brief Find all sides where obj is extending beyond the bounds of box.
   *
   * \return A bitmask of sides relative to obj.
   */
  template <typename T1, typename T2>
  VolumeSides extending_sides(const Volume<T1>& obj,
                              const Volume<T2>& box) noexcept
  {
    GENERATE_VOLUME_BOUNDS(obj);
    GENERATE_VOLUME_BOUNDS(box);

    VolumeSides sides = VolumeSide::None;
    if(obj_top - box_top < 0)
    {
      sides |= VolumeSide::Top;
    }
    if(box_bottom - obj_bottom < 0)
    {
      sides |= VolumeSide::Bottom;
    }
    if(obj_left - box_left < 0)
    {
      sides |= VolumeSide::Left;
    }
    if(box_right - obj_right < 0)
    {
      sides |= VolumeSide::Right;
    }

    return sides;
  }

  /*!
   * \brief Find the side of v1 that is closest to v2.
   *
   * \param v1 Volume whose side is returned.
   * \param v2 Volume which is compared to the other volume.
   *
   * \returns The side of v1 that is closest to v2.
   */
  template <typename T1, typename T2>
  VolumeSides closest_side(const Volume<T1>& v1, const Volume<T2>& v2) noexcept
  {
    // Get a description of the intersecting between the two volumes.
    auto bounds = volume_difference(v1, v2);

    // Find the smallest different between sides and therefore the closest.
    using std::begin; using std::end;
    using pair_type = typename decltype(bounds)::value_type;
    auto iter = std::min_element(begin(bounds), end(bounds),
    [](const pair_type& pair1, const pair_type& pair2)
    {
      return pair1.second < pair2.second;
    });

    // The iterator is not going to be end(bounds) because bounds is non empty.
    return iter->first;
  }

  /*!
   * \brief Find whether obj is inside box (inclusively).
   *
   * \param box The bounding box.
   * \param obj The object that needs to be fully contained in box.
   *
   * \returns True if obj is completely contained within the bounds of box.
   * Inclusive.
   */
  template <typename T1, typename T2>
  bool inside(const Volume<T1>& box, const Volume<T2>& obj) noexcept
  {
    GENERATE_VOLUME_BOUNDS(box);
    GENERATE_VOLUME_BOUNDS(obj);

    if(is_in(box_left, box_right, obj_left) &&
       is_in(box_left, box_right, obj_right) &&
       is_in(box_top, box_bottom, obj_top) &&
       is_in(box_top, box_bottom, obj_bottom))
    {
      return true;
    }
    return false;
  }

  /*!
   * \brief Find the distance of each side of v1 to each side of v2.
   *
   * \param v1 The volume used as a reference.
   * \param v2 The volume compared against v1 whose differences are
   * reported.
   */
  template <typename T1, typename T2>
  std::unordered_map<VolumeSides, decltype(T2() - T1())>
  volume_difference(const Volume<T1>& v1, const Volume<T2>& v2) noexcept
  {
    using map_type = std::unordered_map<VolumeSides,
                                        decltype(T2() - T1())>;

    // Generate bounds.
    GENERATE_VOLUME_BOUNDS(v1);
    GENERATE_VOLUME_BOUNDS(v2);

    map_type map;

    using std::abs;
    map.emplace(VolumeSide::Top, abs(v2_bottom - v1_top));
    map.emplace(VolumeSide::Bottom, abs(v1_bottom - v2_top));
    map.emplace(VolumeSide::Left, abs(v2_right - v1_left));
    map.emplace(VolumeSide::Right, abs(v1_right - v2_left));

    return map;
  }

  /*!
   * \brief Find the delta required to move v to side s of volume ref.
   *
   * \param v The volume that needs to be moved.
   * \param s The side(s) of ref which v needs to align to.
   * \param ref The reference volume.
   */
  template <typename T>
  Vec<T>
  outside_snap(const Volume<T>& v, VolumeSides s, const Volume<T>& ref) noexcept
  {
    Vec<T> pos{v.pos};

    GENERATE_VOLUME_BOUNDS(ref);

    if(s & VolumeSide::Top)
    {
      pos.y = ref_top - v.height;
    }
    if(s & VolumeSide::Bottom)
    {
      pos.y = ref_bottom + 1;
    }
    if(s & VolumeSide::Left)
    {
      pos.x = ref_left - v.width;
    }
    if(s & VolumeSide::Right)
    {
      pos.x = ref_right + 1;
    }

    return pos - v.pos;
  }

  /*!
   * \brief Find the delta required to move v to side s of volume ref.
   *
   * \param v Assumed this is the volume that needs to be moved.
   * \param s The side or sides of ref that v needs to align to.
   * \param ref The reference. A side of this object is to be the destination.
   */
  template <typename T>
  Vec<T>
  inside_snap(const Volume<T>& v, VolumeSides s, const Volume<T>& ref) noexcept
  {
    Vec<T> pos{v.pos};

    GENERATE_VOLUME_BOUNDS(ref);

    if(s & VolumeSide::Top)
    {
      pos.y = ref_top;
    }
    if(s & VolumeSide::Bottom)
    {
      pos.y = ref_bottom - v.height + 1;
    }
    if(s & VolumeSide::Left)
    {
      pos.x = ref_left;
    }
    if(s & VolumeSide::Right)
    {
      pos.x = ref_right - v.width + 1;
    }

    return pos - v.pos;
  }

  /*!
   * \brief Find the four quads that make up the volume v.
   *
   * Array indice reference:
   *  _______
   * |   |   |
   * | 0 | 1 |
   * |___|___|
   * |   |   |
   * | 2 | 3 |
   * |___|___|
   */
  template <typename T>
  std::array<Volume<T>, 4> volume_quads(const Volume<T>& v) noexcept
  {
    std::array<Volume<T>, 4> a;

    GENERATE_VOLUME_BOUNDS(v);

    Vec<T> mid = {v.pos.x + v.width / 2, v.pos.y + v.height / 2};

    a[0] = {v.pos, v.width / 2, v.height / 2};
    a[1] = {{mid.x, v.pos.y}, v.pos.x + v.width - mid.x, v.height / 2};
    a[2] = {{v.pos.x, mid.y}, v.width / 2, v.pos.y + v.height - mid.y};
    a[3] = {mid, v.pos.x + v.width - mid.x, v.pos.y + v.height - mid.y};

    return a;
  }

  /*!
   * \brief Find the center point of a given volume.
   */
  template <typename T>
  inline Vec<T> center(const Volume<T>& v) noexcept
  {
    GENERATE_VOLUME_BOUNDS(v);
    return {v_left + v.width / 2, v_top + v.height / 2};
  }

  /*!
   * \brief Find the normalized vector representing the direction from origin
   * to target.
   */
  template <typename T1, typename T2>
  inline Vec<decltype(T2() - T1())>
  vector_towards(const Volume<T1>& origin, const Volume<T2>& target) noexcept
  {
    return normalize(center(target) - center(origin));
  }
}
