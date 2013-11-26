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
#include <cmath>
#include "collision_util.h"
#include "ConcreteServer.h"
namespace pong
{
  namespace
  {
    void stepBall(Ball& ball, const math::vector<>& vel)
    {
      ball.pos.x += vel.x;
      ball.pos.y += vel.y;
    }
    void stepBall(std::pair<Ball, math::vector<> >& pair)
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

  /*!
   * \brief Returns whether the third parameter is in between the first two
   * parameters.
   */
  bool isIn(int left, int right, int check) noexcept
  {
    return std::min(left, right) <= check && check <= std::max(left, right);
  }

  //                       .- Ball
  //----------------------##------------------------
  //----------------------##------------------------
  //------------------##########--------------------
  //------------------##########--------------------
  //                       *- Paddle
  // This scenario returns true.
  // ...
  //
  //                             .- Ball
  //----------------------------##
  //------------------############------------------
  //------------------##########--------------------
  //                      *- Paddle
  // This scenario will also return true.
  // ...
  // Intersection does not return true.
  // etc.

  //Ignore above, this function returns where the ball is, in relation to the
  //paddle, a result of CollisionSide::TopLeftCorner means the *ball* is
  //located above and left of the paddle!
  CollisionSide paddleIsBesideBall(const Paddle& paddle, const Ball& ball)
                                                                       noexcept
  {
    //We have five types of collisions to check for.
    //Paddle-Top to Ball-Bottom.
    //Paddle-Bottom to Ball-Top.
    //Paddle-Left to Ball-Right.
    //Paddle-Right to Ball-Left.
    //Corner-to-Corner cases.

    //Subtracting one from the other, calculated values is necessary so that
    //the width doesn't include points one more than they should.
    int16_t paddle_top = paddle.pos.y;
    int16_t paddle_bottom = paddle.pos.y + paddle.height - 1;
    int16_t paddle_left = paddle.pos.x;
    int16_t paddle_right = paddle.pos.x + paddle.width - 1;
    int16_t ball_bottom = ball.pos.y + ball.diameter - 1;
    int16_t ball_top = ball.pos.y;
    int16_t ball_right = ball.pos.x + ball.diameter - 1;
    int16_t ball_left = ball.pos.x;

    if(isIn(paddle_left, paddle_right, ball_left) ||
       isIn(paddle_left, paddle_right, ball_right) ||
       (ball_left <= paddle_left + paddle.width / 2 &&
        paddle_right - paddle.width / 2 <= ball_right))
    {
      if(paddle_top - 1 == ball_bottom)
      {
        return CollisionSide::Top;
      }
      else if(paddle_bottom + 1 == ball_top)
      {
        return CollisionSide::Bottom;
      }
    }
    if(isIn(paddle_top, paddle_bottom, ball_top) ||
       isIn(paddle_top, paddle_bottom, ball_bottom) ||
       (ball_top <= paddle_top + paddle.height / 2 &&
        paddle_bottom - paddle.height / 2 <= ball_bottom))
    {
      if(paddle_left - 1 == ball_right)
      {
        return CollisionSide::Left;
      }
      else if(paddle_right + 1 == ball_left)
      {
        return CollisionSide::Right;
      }
    }

    //Check the corners.
    //TODO This will do for now, but fix the code duplication somehow.
    if(paddle_top - 1 == ball_bottom)
    {
      if(ball_left - 1 == paddle_right)
      {
        return CollisionSide::TopRightCorner;
      }
      if(ball_right + 1 == paddle_left)
      {
        return CollisionSide::TopLeftCorner;
      }
    }
    else if(paddle_bottom + 1 == ball_top)
    {
      if(ball_left - 1 == paddle_right)
      {
        return CollisionSide::BottomRightCorner;
      }
      if(ball_right + 1 == paddle_left)
      {
        return CollisionSide::BottomLeftCorner;
      }
    }

    return CollisionSide::None;
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

    math::vector<double> velocity_left = std::get<1>(this->ball_);

    for(double i = 0; i < math::length(velocity_left); i += .1)
    {
      decltype(ball.pos) next_position = velocity_left * i;
      Ball ball_copy(ball.id, next_position, ball.diameter);

      auto first_paddle_collision_result =
                                   paddleIsBesideBall(first_paddle, ball_copy);
      if(CollisionSide::None != first_paddle_collision_result)
      {
        if(CollisionSide::Top == first_paddle_collision_result ||
           CollisionSide::Bottom == first_paddle_collision_result)
        {
          //Flip the y component of the original velocity vector.
          std::get<1>(this->ball_).y = -std::get<1>(this->ball_).y;
          velocity_left.y = -velocity_left.y;
        }
        else if(CollisionSide::Left == first_paddle_collision_result ||
           CollisionSide::Right == first_paddle_collsion_result)
        {
          std::get<1>(this->ball_).x = -std::get<1>(this->ball_).x;
          velocity_left.x = -velocity_left.x;
        }
        //Corner case!
        else
        {
          std::get<1>(this->ball_) = -std::get<1>(this->ball_);
          velocity_left = -velocity_left;
        }

        //Subtract what we already have done.
        velocity_left = velocity_left - i * velocity_left;

        //Reset i.
        i = 0.0;
        continue;
      }
      //Okay no, collision, then just keep moving.
    }
  }
};
