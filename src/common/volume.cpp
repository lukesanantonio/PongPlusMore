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
#include "volume.h"
namespace pong
{
  /*!
   * \return A bitmask of sides relative to obj.
   */
  VolumeSides extending_sides(const Volume& obj, const Volume& box) noexcept
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
   * \returns True if the two volumes intersect in any way, false otherwise.
   */
  bool intersecting(const Volume& v1, const Volume& v2) noexcept
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
   * \param v1 The volume used as a reference.
   * \param v2 The volume compared against v1 whose differences are
   * reported.
   */
  std::unordered_map<VolumeSides, double>
  volume_difference(const Volume& v1, const Volume& v2) noexcept
  {
    using map_type = std::unordered_map<VolumeSides, double>;

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
   * \param v1 Volume whose side is returned.
   * \param v2 Volume which is compared to the other volume.
   *
   * \returns The side of v1 that is closest to v2.
   */
  VolumeSides closest_side(const Volume& v1, const Volume& v2) noexcept
  {
    // Get a description of the intersecting between the two volumes.
    auto bounds = volume_difference(v1, v2);

    // Find the smallest different between sides and therefore the closest.
    using std::begin; using std::end;
    using pair_type = decltype(bounds)::value_type;
    auto iter = std::min_element(begin(bounds), end(bounds),
    [](const pair_type& pair1, const pair_type& pair2)
    {
      return pair1.second < pair2.second;
    });

    // The iterator is not going to be end(bounds) because bounds is non empty.
    return iter->first;
  }

  /*!
   * \param box The bounding box.
   * \param obj The object that needs to be fully contained in box.
   *
   * \returns True if obj is completely contained within the bounds of box.
   * Inclusive.
   */
  bool inside(const Volume& box, const Volume& obj) noexcept
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
   * \param v The volume that needs to be moved.
   * \param s The side(s) of ref which v needs to align to.
   * \param ref The reference volume.
   */
  math::vector<double>
  outside_snap(const Volume& v, VolumeSides s, const Volume& ref) noexcept
  {
    math::vector<double> pos{v.pos};

    GENERATE_VOLUME_BOUNDS(ref);

    if(s & VolumeSide::Top)
    {
      pos.y = ref_top - v.height + 1;
    }
    if(s & VolumeSide::Bottom)
    {
      pos.y = ref_bottom + 1;
    }
    if(s & VolumeSide::Left)
    {
      pos.x = ref_left - v.width + 1;
    }
    if(s & VolumeSide::Right)
    {
      pos.x = ref_right + 1;
    }

    return pos - v.pos;
  }

  /*!
   * \param v Assumed this is the volume that needs to be moved.
   * \param s The side or sides of ref that v needs to align to.
   * \param ref The reference. A side of this object is to be the destination.
   */
  math::vector<double>
  inside_snap(const Volume& v, VolumeSides s, const Volume& ref) noexcept
  {
    math::vector<double> pos{v.pos};

    GENERATE_VOLUME_BOUNDS(ref);

    if(s & VolumeSide::Top)
    {
      pos.y = ref_top + 1;
    }
    if(s & VolumeSide::Bottom)
    {
      pos.y = ref_bottom - v.height + 1;
    }
    if(s & VolumeSide::Left)
    {
      pos.x = ref_left + 1;
    }
    if(s & VolumeSide::Right)
    {
      pos.x = ref_right - v.width + 1;
    }

    return pos - v.pos;
  }

  // Array indice reference:
  //  _______
  // |   |   |
  // | 0 | 1 |
  // |___|___|
  // |   |   |
  // | 2 | 3 |
  // |___|___|
  std::array<Volume, 4> volume_quads(const Volume& v) noexcept
  {
    std::array<Volume, 4> a;

    GENERATE_VOLUME_BOUNDS(v);

    math::vector<double> mid = {v.pos.x + v.width / 2,
                                v.pos.y + v.height / 2};

    a[0] = Volume(v.pos, v.width / 2, v.height / 2);
    a[1] = Volume({mid.x, v.pos.y}, v.pos.x + v.width - mid.x, v.height / 2);
    a[2] = Volume({v.pos.x, mid.y}, v.width / 2, v.pos.y + v.height - mid.y);
    a[3] = Volume(mid, v.pos.x + v.width - mid.x, v.pos.y + v.height - mid.y);

    return a;
  }
}
