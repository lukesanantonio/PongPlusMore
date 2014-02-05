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
 *
 * \file collision_util.h
 * \brief Collision helper functions.
 */
#pragma once
#include <algorithm>
#include <unordered_map>
#include <vector>
#include "common/vector.h"
#include "common/Volume.h"
#include "World.h"
#include "util.h"
#include "Object.h"
#include "enum_hash.hpp"
namespace pong
{
  inline bool isIn(int left, int right, int check) noexcept
  {
    return std::min(left, right) <= check && check <= std::max(left, right);
  }
  bool isIntersecting(const Volume& vol1, const Volume& vol2) noexcept;

  bool isIntersectingWithWall(const Volume& vol,
                              int width, int height) noexcept;

  std::vector<math::vector<int> >
  raytrace(math::vector<double> ray) noexcept;

  auto raytrace(math::vector<double> ray, math::vector<int> start)
                                                    -> decltype(raytrace(ray));

  enum class VolumeSide : unsigned int
  {
    Top, Bottom, Left, Right, None
  };

  template <typename key_type, typename value_type>
  using unordered_map_enumhash = std::unordered_map<key_type, value_type,
                                                    enum_hash<key_type> >;

  VolumeSide findClosestSide(const Volume& vol1, const Volume& vol2) noexcept;

  unordered_map_enumhash<VolumeSide, int>
  getVolumePenetration(const Volume& vol1, const Volume& vol2) noexcept;
}
