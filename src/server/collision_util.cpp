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
#include "collision_util.h"
namespace pong
{
  /*!
   * \brief Returns the boolean state of intersection between two volumes.
   *
   * \param v1 A volume.
   * \param v2 A volume.
   *
   * \returns True if the two volumes intersect somehow. False otherwise.
   */
  bool isIntersecting(const Volume& v1, const Volume& v2) noexcept
  {
    GENERATE_VOLUME_BOUNDS(v1);
    GENERATE_VOLUME_BOUNDS(v2);

    if(isIn(v1_left, v1_right, v2_left) or
       isIn(v1_left, v1_right, v2_right) or
       (v2_left <= v1_left && v1_right <= v2_right))
    {
      if(isIn(v1_top, v1_bottom, v2_top) or
         isIn(v1_top, v1_bottom, v2_bottom) or
         (v2_top <= v1_top && v1_bottom <= v2_bottom)) return true;
    }

    return false;
  }

  /*!
   * \brief Returns the distance of each side from v2 in a map.
   *
   * \param v1 The volume used as a reference.
   * \param v2 The volume compared against v1 whose differences are
   * reported.
   *
   * \returns A hash map describing how much each side of v2 penetrates v1.
   * Values will only be greater than 0.
   */
  std::unordered_map<VolumeSides, double>
  getVolumePenetration(const Volume& v1, const Volume& v2) noexcept
  {
    using map_type = std::unordered_map<VolumeSides, double>;

    // If the volumes are not intersecting the results are irrelevant.
    if(!isIntersecting(v1, v2)) return map_type{};

    // Generate bounds.
    GENERATE_VOLUME_BOUNDS(v1);
    GENERATE_VOLUME_BOUNDS(v2);

    map_type map;

    // Find the actual penetration.
    using std::max;
    map.emplace(VolumeSide::Top, max(v2_bottom - v1_top, -1.0));
    map.emplace(VolumeSide::Bottom, max(v1_bottom - v2_top, -1.0));
    map.emplace(VolumeSide::Left, max(v2_right - v1_left, -1.0));
    map.emplace(VolumeSide::Right, max(v1_right - v2_left, -1.0));


    // Remove the ones that don't even intersect.
    using std::begin; using std::end;

    for(auto iter = begin(map); iter != end(map);)
    {
      if(iter->second < 0) iter = map.erase(iter);
      else ++iter;
    }

    return map;
  }

  /*!
   * \brief Finds the side of the first volume that is closest to the other
   * volume.
   *
   * \param v1 This is the volume whose side is returned.
   * \param v2 This is the volume which is compared to the other volume.
   *
   * \returns The side of v1 that is closest to v2.
   */
  VolumeSides findClosestSide(const Volume& v1, const Volume& v2) noexcept
  {
    // Get a description of the intersecting between the two volumes.
    auto bounds = getVolumePenetration(v1, v2);

    // Wait, no intersecting? Screw it!
    if(bounds.empty()) return VolumeSide::None;

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
   * \brief Returns whether obj is inside box. Inclusive.
   *
   * \param box The bounding box.
   * \param obj The object that needs to be fully contained in box.
   *
   * \returns True if obj is completely contained within the bounds of box.
   * Inclusive.
   */
  bool isInsideVolume(const Volume& box, const Volume& obj) noexcept
  {
    GENERATE_VOLUME_BOUNDS(box);
    GENERATE_VOLUME_BOUNDS(obj);

    if(isIn(box_left, box_right, obj_left) &&
       isIn(box_left, box_right, obj_right) &&
       isIn(box_top, box_bottom, obj_top) &&
       isIn(box_top, box_bottom, obj_bottom))
    {
      return true;
    }
    return false;
  }

  /*!
   * \brief Returns all sides where the obj is extending beyond the bounds of
   * box.
   */
  std::vector<VolumeSides> allProtrudingSides(const Volume& obj,
                                             const Volume& box) noexcept
  {
    std::unordered_map<VolumeSides, double> map;

    GENERATE_VOLUME_BOUNDS(obj);
    GENERATE_VOLUME_BOUNDS(box);

    map.emplace(VolumeSide::Top, obj_top - box_top);
    map.emplace(VolumeSide::Bottom, box_bottom - obj_bottom);
    map.emplace(VolumeSide::Left, obj_left - box_left);
    map.emplace(VolumeSide::Right, box_right - obj_right);

    using std::begin; using std::end;

    // Remove the bad ones.
    for(auto iter = begin(map); iter != end(map);)
    {
      double var = iter->second;
      if(var >= 0)
      {
        iter = map.erase(iter);
      }
      else ++iter;
    }

    std::vector<VolumeSides> sides;
    for(auto pair : map)
    {
      sides.push_back(pair.first);
    }
    return sides;
  }

  /*!
   * \brief Snaps a volume to the side of another volume.
   *
   * \param to_move The volume to modify.
   * \param side The side of v to snap to_move to.
   * \param v The reference volume, not modified (well, const).
   *
   * \note This function doesn't check to make sure to_move is actually *next*
   * to v after the snap. Meaning a snap to the top of v will only cause
   * to_move's Y component to be changed appropriately.
   */
  void snapVolumeToVolume(Volume& to_move, VolumeSides side,
                          const Volume& v) noexcept
  {
    switch(side)
    {
      case VolumeSide::Top:
      {
        to_move.pos.y = v.pos.y - to_move.height;
        break;
      }
      case VolumeSide::Bottom:
      {
        to_move.pos.y = v.pos.y + v.height;
        break;
      }
      case VolumeSide::Left:
      {
        to_move.pos.x = v.pos.x - to_move.width;
        break;
      }
      case VolumeSide::Right:
      {
        to_move.pos.x = v.pos.x + v.width;
        break;
      }
      default:
        break;
    }
  }

  /*!
   * \brief Returns the required difference needed to snap to a side of v.
   *
   * \param to_move Assumed this is the volume that needs to be moved.
   * \param side The side that to_move needs to snap to.
   * \param v The reference. A side of this object is the destination.
   *
   * \returns A diff from where to_move is and where it needs to be.
   */
  math::vector<double> snapDiff(const Volume& to_move, VolumeSides side,
                                const Volume& v) noexcept
  {
    Volume to_move_copy = to_move;
    snapVolumeToVolume(to_move_copy, side, v);
    return to_move_copy.pos - to_move.pos;
  }

  /*!
   * \brief Returns the required distance to snap a volume to the side of
   * another from the inside of that volume.
   *
   * \param to_move Assumed this is the volume that needs to be moved.
   * \param side The side that to_move needs to snap to.
   * \param v The reference. A side of this object is the destination.
   *
   * It ends up being that to_move's side is the same side as the parameter.
   */
  math::vector<double> snapToVolumeInsideDiff(const Volume& to_move,
                                              VolumeSides side,
                                              const Volume& v) noexcept
  {
    math::vector<double> pos{to_move.pos};

    GENERATE_VOLUME_BOUNDS(v);

    switch(side)
    {
      case VolumeSide::Top:
      {
        pos.y = v_top;
        break;
      }
      case VolumeSide::Bottom:
      {
        pos.y = v_bottom - to_move.height + 1;
        break;
      }
      case VolumeSide::Left:
      {
        pos.x = v_left;
        break;
      }
      case VolumeSide::Right:
      {
        pos.x = v_right - to_move.width + 1;
        break;
      }
      default: break;
    }
    return pos - to_move.pos;
  }

  // Array indice reference:
  //  _______
  // |   |   |
  // | 0 | 1 |
  // |___|___|
  // |   |   |
  // | 2 | 3 |
  // |___|___|
  std::array<Volume, 4> find_volume_quads(const Volume& vol) noexcept
  {
    std::array<Volume, 4> a;

    GENERATE_VOLUME_BOUNDS(vol);

    math::vector<double> mid = {vol.pos.x + vol.width / 2,
                                vol.pos.y + vol.height / 2};

    a[0] = Volume(vol.pos,
                  vol.width / 2,
                  vol.height / 2);

    a[1] = Volume({mid.x, vol.pos.y},
                  vol.pos.x + vol.width - mid.x,
                  vol.height / 2);

    a[2] = Volume({vol.pos.x, mid.y},
                  vol.width / 2,
                  vol.pos.y + vol.height - mid.y);

    a[3] = Volume(mid,
                  vol.pos.x + vol.width - mid.x,
                  vol.pos.y + vol.height - mid.y);

    return a;
  }
}
