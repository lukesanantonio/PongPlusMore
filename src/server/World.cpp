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
#include "World.h"
#include <algorithm>
#include "util.h"
#include "exceptions.h"
namespace pong
{
  const Paddle& findPaddleByID(const World& world, PaddleID id)
  {
    return findObjectByID(world.paddles, id);
  }
  Paddle& findPaddleByID(World& world, PaddleID id)
  {
    // We know we can modify the result. The World is non-const, the vector
    // is non-const, etc.
    return const_cast<Paddle&>(findObjectByID(world.paddles, id));
  }
}
