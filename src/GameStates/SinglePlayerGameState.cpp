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
/*!
 * \file SinglePlayerGameState.cpp
 * \brief Contains definitions for the SinglePlayerGameState GameState.
 */
#include "SinglePlayerGameState.h"
#include "Game.h"
#include "PaddleControllers/MousePaddleController.h"
#include "PaddleControllers/UnbeatableAIPaddleController.h"
#include "GameStates/MenuGameState.h"
#include "render_text.h"
#include "Physics/SimplePhysicsWorld.h"
namespace pong
{
  SinglePlayerGameState::SinglePlayerGameState(Game& game) noexcept :
                                                                 game_(game)
  {
    //Initialize paddles (positions, etc).
    this->topPaddle_.position({static_cast<double>(center(0, game.width(),
                                                   this->topPaddle_.width())),
                               0});

    this->bottomPaddle_.position(
    //                                ,--This is casted to a double here to
    //                               /   avoid narrowing conversion warnings. I
    //                              \/   don't really like it but, eh.
                    {0,static_cast<double>(game.height()
                                        - this->bottomPaddle_.height())});

    //Set initial position of the ball. The center.
    math::vector ball_pos;
    ball_pos.x = center(0, game.width(),
                        this->ball_.diameter());
    ball_pos.y = center(0, game.height(),
                        this->ball_.diameter());
    this->ball_.position(ball_pos);

    //Add the objects to the render queue.
    this->addRenderableEntity(&this->topPaddle_);
    this->addRenderableEntity(&this->bottomPaddle_);
    this->addRenderableEntity(&this->ball_);

    //Configure the mouse.
    SDL_WM_GrabInput(SDL_GRAB_ON);
    SDL_ShowCursor(SDL_DISABLE);
  }

  void SinglePlayerGameState::handleEvent(const SDL_Event& event) noexcept
  {
    if(event.type == SDL_KEYUP)
    {
      this->game_.popGameState();
    }
  }
};
