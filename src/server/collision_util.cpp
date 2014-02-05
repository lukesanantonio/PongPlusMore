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
  #define GENERATE_VOLUME_BOUNDS(vol) \
    int vol##_left = vol.pos.x, \
        vol##_right = vol.pos.x + vol.width - 1, \
        vol##_top = vol.pos.y, \
        vol##_bottom = vol.pos.y + vol.height - 1

  bool isIntersecting(const Volume& v1, const Volume& v2) noexcept
  {
    GENERATE_VOLUME_BOUNDS(v1);
    GENERATE_VOLUME_BOUNDS(v2);

    if((v2_left <= v1_left and v1_left <= v2_right) or
       (v2_left <= v1_right and v1_right <= v2_right) or
       (v1_left <= v2_left and v2_right <= v1_right))
    {
      if((v2_top <= v1_top and v1_top <= v2_bottom) or
         (v2_top <= v1_bottom and v1_bottom <= v2_bottom) or
         (v1_top <= v2_top and v2_bottom <= v1_bottom)) return true;
    }
    return false;
  }

  /*!
   * \brief Returns all discrete positions along a ray.
   */
  auto raytrace(math::vector<double> ray) noexcept
                                                  -> std::vector<decltype(ray)>
  {
    math::vector<double> direction = math::normalize(ray);
    auto length = math::length(direction);

    std::vector<math::vector<double> > points{{0.0, 0.0}};

    for(double i = 0.0; i <= length; i += length / std::ceil(length))
    {
      math::vector<double> point = direction * i;
      if(std::find(points.begin(), points.end(), point) == points.end())
      {
        points.push_back(std::move(point));
      }
    }

    return points;
  }

  auto raytrace(math::vector<double> ray, math::vector<double> start)
                                                     -> decltype(raytrace(ray))
  {
    auto ray_points = raytrace(ray);

    using std::begin; using std::end;
    std::transform(begin(ray_points), end(ray_points), begin(ray_points),
                [&](const math::vector<double>& point){return point + start;});
    return ray_points;
  }

  /*!
   * \brief Finds the side of the first volume that is closest to some side of
   * the second volume.
   *
   * \param v1 This is the volume whose side is returned.
   * \param v2 This is the volume which is compared to the other volume.
   *
   * \returns The side of v1 that is closest to v2.
   */
  VolumeSide findClosestSide(const Volume& v1, const Volume& v2) noexcept
  {

    auto bounds = getVolumePenetration(v1, v2);

    if(bounds.empty()) return VolumeSide::None;

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
   * \brief Returns the distance of each side from v2 in a map.
   *
   * \param v1 The volume used as a reference
   * \param v2 The volume compared against v1 whose differences are
   * reported.
   *
   * \returns A hash map describing how much each side of v2 penetrates v1.
   * Value integers will be only greater than 0.
   */
  unordered_map_enumhash<VolumeSide, double>
  getVolumePenetration(const Volume& v1, const Volume& v2) noexcept
  {
    using map_type = unordered_map_enumhash<VolumeSide, double>;

    // If the volumes are not intersecting the results are irrelevant.
    if(!isIntersecting(v1, v2)) return map_type{};

    GENERATE_VOLUME_BOUNDS(v1);
    GENERATE_VOLUME_BOUNDS(v2);

    map_type map;

    using std::max;
    map.emplace(VolumeSide::Top, max(v2_bottom - v1_top, -1));
    map.emplace(VolumeSide::Bottom, max(v1_bottom - v2_top, -1));
    map.emplace(VolumeSide::Left, max(v2_right - v1_left, -1));
    map.emplace(VolumeSide::Right, max(v1_right - v2_left, -1));


    // Remove the ones that don't even intersect.
    using std::begin; using std::end;

    for(auto iter = begin(map); iter != end(map);)
    {
      if(iter->second < 0) iter = map.erase(iter);
      else ++iter;
    }

    return map;
  }
}
