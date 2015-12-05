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
namespace pong
{
  /*!
   * \brief Gets the point which should be the location of an object of
   * `object_length` with the bounds which begin at `begin` and go on for
   * `length`.
   *
   * \param begin The location where the line, etc begins.
   * \param length How far this line, etc, goes for, after `begin`.
   * \param object_length The length of the object which needs to be centered.
   *
   * \return The 1d point which should be the position of an object of length
   * `object_length` if it needs to be centered in a line starting at point
   * `begin` going on for `length` amount.
   *
   * \note Example: If begin==100, length==100, and object_length==50. This
   * function returns 125. This would be the x or y point for on object of
   * length 50 which needs to be centered within the bounds 100-200.
   *
   * \note The algorithm is exactly:
   * \code begin + (length / 2) - (object_length / 2) \endcode
   * This is what is returned.
   */
  template<typename P1, typename P2, typename P3>
  inline constexpr decltype(auto)
  center(P1 begin, P2 length, P3 object_length)
  {
    return begin + (length / 2) - (object_length / 2);
  }
}
