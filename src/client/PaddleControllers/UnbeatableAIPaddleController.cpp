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
#include "UnbeatableAIPaddleController.h"
#include "Paddle.h"
namespace pong
{
  void UnbeatableAIPaddleController::update(Paddle* paddle)
  {
    if(!this->tracked_ball) return;

    math::vector new_position = paddle->position();

    auto ball_center_x = this->tracked_ball->position().x +
                         this->tracked_ball->diameter() / 2;
    new_position.x = ball_center_x - paddle->width() / 2;
    paddle->position(new_position);
  }
};
