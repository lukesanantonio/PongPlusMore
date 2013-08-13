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
#include "PhysicsWorld.h"
namespace pong
{
  void PhysicsWorld::addBall(Ball* ball, math::vector velocity)
  {
    BallWrapper ball_wrapper;

    ball_wrapper.ball = ball;
    ball_wrapper.velocity = velocity;

    this->balls_.push_back(ball_wrapper);
  }
  void PhysicsWorld::addPaddle(Paddle* paddle,
                               std::shared_ptr<PaddleController> controller)
  {
    PaddleWrapper paddle_wrapper;

    paddle_wrapper.paddle = paddle;
    paddle_wrapper.controller = controller;

    this->paddles_.push_back(paddle_wrapper);
  }
};
