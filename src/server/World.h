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
#include <vector>
#include "Paddle.h"
#include "Ball.h"
#include "util.h"
namespace pong
{
  struct World
  {
    std::vector<Paddle> paddles;
    std::vector<Ball> balls;
  };

  inline const Paddle& findPaddleByID(const World& world, PaddleID id)
  {
    return findObjectByID(world.paddles, id);
  }
  inline Paddle& findPaddleByID(World& world, PaddleID id)
  {
    // We know we can modify the result. The World is non-const, the vector
    // is non-const, and the elements are non-const.
    return const_cast<Paddle&>(findObjectByID(world.paddles, id));
  }

  inline const Ball& findBallByID(const World& world, BallID id)
  {
    return findObjectByID(world.balls, id);
  }
  inline Ball& findBallByID(World& world, BallID id)
  {
    return const_cast<Ball&>(findObjectByID(world.balls, id));
  }
}
