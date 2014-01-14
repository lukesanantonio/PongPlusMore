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
#include "PaddleGameState.h"
#include "render.h"
#include "common/crash.hpp"
namespace pong
{
  void PaddleGameState::handleEvent(const SDL_Event& event)
  {
    switch(event.type)
    {
      case SDL_MOUSEBUTTONDOWN:
      {
        if(top_) break;
        top_ = this->server_.connect();
        break;
      }
      case SDL_MOUSEMOTION:
      {
        if(!this->top_) break;
        this->server_.getPaddle(this->top_).pos =
                                              {event.motion.x, event.motion.y};
        break;
      }
      case SDL_QUIT:
      {
        pong::crash("Exiting...");
      }
    }
  }

  void PaddleGameState::update()
  {
  }

  void PaddleGameState::render(SDL_Renderer* renderer) const
  {
    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
    for(Paddle paddle : this->server_.paddles())
    {
      pong::render(renderer, paddle);
    }
  }
}
