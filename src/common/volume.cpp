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
   * \brief Return all sides where obj is extending beyond the bounds of box.
   *
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
}
