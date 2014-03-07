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
#include "enum_hash.hpp"
#include "ObjectManager.h"
namespace pong
{
  /*!
   * \brief Returns whether or not check is inside a range inclusive.
   *
   * \param left One end of the range.
   * \param right Another end of the range.
   * \param check Number to check.
   *
   * \returns True if check is between left..right or right..left
   * inclusive.
   */
  template <typename P1, typename P2, typename P3>
  constexpr inline bool isIn(P1 left, P2 right, P3 check) noexcept
  {
    return std::min(left, right) <= check && check <= std::max(left, right);
  }

  bool isIntersecting(const Volume& v1, const Volume& v2) noexcept;

  enum class VolumeSide : unsigned int
  {
    Top, Bottom, Left, Right, None
  };

  VolumeSide findClosestSide(const Volume& v1, const Volume& v2) noexcept;

  bool isInsideVolume(const Volume& box, const Volume& obj) noexcept;
  VolumeSide closestSideFromInside(const Volume& box,
                                   const Volume& obj) noexcept;

  void snapVolumeToVolume(Volume&, VolumeSide, const Volume&) noexcept;
  math::vector<double> snapDiff(const Volume&, VolumeSide,
                                const Volume&) noexcept;
}
