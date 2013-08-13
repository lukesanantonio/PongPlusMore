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
  SinglePlayerGameState::SinglePlayerGameState()
  {
    //Initialize paddles (positions, etc).
    this->topPaddle_.position({0, 0});

    this->bottomPaddle_.position(
    //                                ,--This is casted to a double here to
    //                               /   avoid narrowing conversion warnings. I
    //                              \/   don't really like it but, eh.
                    {0,static_cast<double>(Game::getInstance()->height()
                                        - this->bottomPaddle_.height())});

    //Initialize the physics world.
    this->world_.addPaddle(&this->topPaddle_,
                  std::make_shared<UnbeatableAIPaddleController>(&this->ball_));
    this->world_.addPaddle(&this->bottomPaddle_,
                           std::make_shared<MousePaddleController>());

    this->world_.addBall(&this->ball_, {0, 5});

    //Set initial position of the ball. The center.
    math::vector ball_pos;
    ball_pos.x = center(0, Game::getInstance()->width(),
                        this->ball_.diameter());
    ball_pos.y = center(0, Game::getInstance()->height(),
                        this->ball_.diameter());
    this->ball_.position(ball_pos);

    //Configure the mouse.
    SDL_WM_GrabInput(SDL_GRAB_ON);
    SDL_ShowCursor(SDL_DISABLE);
  }

  void SinglePlayerGameState::update_private()
  {
    if(Game::getInstance()->events.keyHasBeenPressed())
    {
      Game::getInstance()->setGameState(std::shared_ptr<GameState>(
                                                            new MenuGameState));
    }
    this->world_.step();
  }
  void SinglePlayerGameState::render_private(SDL_Surface* surface) const
  {
    this->topPaddle_.render(surface);
    this->bottomPaddle_.render(surface);
    this->ball_.render(surface);
  }
};
