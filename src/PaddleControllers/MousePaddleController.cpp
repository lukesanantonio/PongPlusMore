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
#include "MousePaddleController.h"
#include "Paddle.h"
#include "Game.h"
namespace pong
{
  void MousePaddleController::update(Paddle* paddle)
  {
    //Get the game instance.
    Game* game = Game::getInstance();

    //Position calculation.
    math::vector pos = {
         game->events.mousePosition().x - static_cast<int>(paddle->width()) / 2,
         paddle->position().y};

    //Bounds constraints.
    if(pos.x < 0) pos.x = 0;
    if(pos.x > game->width() - paddle->width())
    {
      pos.x = game->width() - paddle->width();
    }

    //Update position in the paddle.
    paddle->position(pos);
  }
}
