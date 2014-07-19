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
#include "common/vector.h"
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
    math::vector<double> pos;
    double width, height;

    Volume(decltype(pos) pos = decltype(pos){},
           decltype(width) width = 0, decltype(height) height = 0)
           : pos(pos), width(width), height(height) {}
  };

  inline bool operator==(const Volume& v1, const Volume& v2) noexcept
  {
    return v1.pos == v2.pos && v1.width == v2.width && v1.height == v2.height;
  }
}
