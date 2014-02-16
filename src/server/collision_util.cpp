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
   * \brief A macro for generating local variables of Volume bounds.
   */
  #define GENERATE_VOLUME_BOUNDS(vol) \
    double vol##_left = vol.pos.x, \
           vol##_right = vol.pos.x + vol.width - 1, \
           vol##_top = vol.pos.y, \
           vol##_bottom = vol.pos.y + vol.height - 1

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
   * \brief An unordered map that can use strongly typed enums as keys.
   */
  template <typename key_type, typename value_type>
  using unordered_map_enumhash = std::unordered_map<key_type, value_type,
                                                    enum_hash<key_type> >;

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
  unordered_map_enumhash<VolumeSide, double>
  getVolumePenetration(const Volume& v1, const Volume& v2) noexcept
  {
    using map_type = unordered_map_enumhash<VolumeSide, double>;

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
  VolumeSide findClosestSide(const Volume& v1, const Volume& v2) noexcept
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
   * \brief Returns the ids of objects that intersect with another object.
   *
   * \param id Id of the reference object from the ObjectManager.
   * \param objs The ObjectManager which contains all the objects to compare to
   * the reference one.
   *
   * \returns Vector of ids;
   */
  std::vector<id_type>
  findIntersectingObjects(id_type id, const ObjectManager& objs) noexcept
  {
    std::vector<id_type> ids;

    const Object& obj = objs.findObject(id);

    for(const auto& pair : objs)
    {
      // If the our object is the same as this one collision detection is moot.
      if(std::get<0>(pair) == id) continue;

      if(isIntersecting(std::get<1>(pair).getVolume(), obj.getVolume()))
      {
        ids.push_back(std::get<0>(pair));
      }
    }

    return ids;
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

  VolumeSide closestSideFromInside(const Volume& box,
                                   const Volume& obj) noexcept
  {
    unordered_map_enumhash<VolumeSide, int> map;

    GENERATE_VOLUME_BOUNDS(box);
    GENERATE_VOLUME_BOUNDS(obj);

    map.emplace(VolumeSide::Top, obj_top - box_top);
    map.emplace(VolumeSide::Bottom, box_bottom - obj_bottom);
    map.emplace(VolumeSide::Left, obj_left - box_left);
    map.emplace(VolumeSide::Right, box_right - obj_right);

    using std::begin; using std::end;

    // Remove the bad ones.
    for(auto iter = begin(map); iter != end(map);)
    {
      if(std::get<1>(*iter) < 0) iter = map.erase(iter);
      ++iter;
    }

    // Find the good one.
    auto it = std::min_element(begin(map), end(map),
    [](const auto& p1, const auto& p2)
    {
      return std::get<1>(p1) < std::get<1>(p2);
    });

    return it == end(map) ? VolumeSide::None : std::get<0>(*it);
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
  void snapVolumeToVolume(Volume& to_move, VolumeSide side,
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
}
