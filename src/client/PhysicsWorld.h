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
#ifndef ULTIMATE_PONG_PHYSICS_WORLD_H
#define ULTIMATE_PONG_PHYSICS_WORLD_H
#include <vector>
#include <memory>
#include "Ball.h"
#include "Paddle.h"
#include "PaddleController.hpp"
namespace pong
{
  class PhysicsWorld
  {
  protected:
    struct BallWrapper
    {
      Ball* ball;
      math::vector velocity;
    };

    struct PaddleWrapper
    {
      Paddle* paddle;
      /*!
       * A smart pointer because it is likely to be the only handle to the
       * controller at most times. We avoid destruction this way.
       */
      std::shared_ptr<PaddleController> controller;
    };
  public:
    virtual ~PhysicsWorld() noexcept = default;
    /*!
     * \brief Adds a ball to the simulation.
     *
     * \param ball A pointer to the ball.
     * \param velocity The initial velocity of the ball.
     */
    void addBall(Ball* ball, math::vector velocity);
    /*!
     * \brief Adds a paddle as a collision mass
     *
     * Paddle's will be modified, but not necessarily by their respective
     * controller, the PhysicsWorld has full discretion.
     */
    void addPaddle(Paddle* paddle,
                   std::shared_ptr<PaddleController> controller);

    /*!
     * \brief Step the physics, modify all balls in the PhysicsWorld::balls_
     * vector, and update all paddles.
     */
    virtual void step() = 0;
  protected:
    std::vector<BallWrapper> balls_;
    std::vector<PaddleWrapper> paddles_;
  };
};
#endif
