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
#include "ConcreteServer.h"
namespace pong
{
  namespace
  {
    void stepBall(Ball& ball, const math::vector& vel)
    {
      ball.pos.x += vel.x;
      ball.pos.y += vel.y;
    }
    void stepBall(std::pair<Ball, math::vector>& pair)
    {
      stepBall(std::get<0>(pair), std::get<1>(pair));
    }

    void stepPaddle(Paddle& paddle, ConcreteServer::paddle_x_type x)
    {
      paddle.pos.x = x;
    }
    void stepPaddle(std::pair<Paddle, ConcreteServer::paddle_x_type>& pair)
    {
      stepPaddle(std::get<0>(pair), std::get<1>(pair));
    }
  }
  void ConcreteServer::step()
  {
    //Don't stand for dealing with freakin' pairs.
    //Just reference the values with local variables.
    Paddle& first_paddle = std::get<0>(this->first_paddle_);
    Paddle& second_paddle = std::get<0>(this->second_paddle_);

    //Move the paddles.
    stepPaddle(this->first_paddle_);
    stepPaddle(this->second_paddle_);

    //Get the paddle
    Ball& ball = std::get<0>(this->ball_);

    //Get the ball moving.
    stepBall(this->ball_);
  }
};
