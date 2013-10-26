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
#include <cstdint>
#include "common/vector.h"
namespace pong
{
  /*!
   * \brief Used to identify a paddle to the server.
   */
  using PaddleID = uint8_t;
  struct Paddle
  {
    Paddle(PaddleID id = 0, math::vector pos = math::vector(),
           uint16_t width = 0, uint16_t height = 0)
         : id(id), pos(pos), width(width), height(height){}
    PaddleID id;
    math::vector pos;
    uint16_t width, height;
  };
}
