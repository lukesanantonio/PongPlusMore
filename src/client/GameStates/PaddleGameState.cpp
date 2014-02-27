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
#include <algorithm>
#include "PaddleGameState.h"
#include "../render.h"
#include "common/crash.hpp"
#include "server/collision_util.h"
#include <json/json.h>
namespace pong
{
  void PaddleGameState::handleEvent(const SDL_Event& event)
  {
    switch(event.type)
    {
      case SDL_MOUSEBUTTONDOWN:
      {
        if(top_) break;
        top_ = this->server_.createPaddle({{0, 0}, 200, 50});
        break;
      }
      case SDL_MOUSEMOTION:
      {
        if(!this->top_) break;
        this->server_.setDestination(this->top_,
                         math::vector<double>(event.motion.x, event.motion.y));
        break;
      }
      case SDL_KEYDOWN:
      {
        switch(event.key.keysym.scancode)
        {
          case SDL_SCANCODE_SPACE:
          {
            this->ball_ = this->server_.createBall({{500,500}, 25, 25});
            this->server_.setVelocity(this->ball_, {0,-.25});
            break;
          }
          case SDL_SCANCODE_Q:
          {
            if(this->bottom_) break;
            bottom_ = this->server_.createPaddle({{0, 0}, 200, 50});
            break;
          }
          case SDL_SCANCODE_W:
          {
            if(!this->bottom_) break;
            auto pos = this->server_.getObject(bottom_).getVolume().pos;
            pos.y -= 35;
            this->server_.setDestination(bottom_, pos);
            break;
          }
          case SDL_SCANCODE_A:
          {
            if(!this->bottom_) break;
            auto pos = this->server_.getObject(bottom_).getVolume().pos;
            pos.x -= 35;
            this->server_.setDestination(bottom_, pos);
            break;
          }
          case SDL_SCANCODE_S:
          {
            if(!this->bottom_) break;
            auto pos = this->server_.getObject(bottom_).getVolume().pos;
            pos.y += 35;
            this->server_.setDestination(bottom_, pos);
            break;
          }
          case SDL_SCANCODE_D:
          {
            if(!this->bottom_) break;
            auto pos = this->server_.getObject(bottom_).getVolume().pos;
            pos.x += 35;
            this->server_.setDestination(bottom_, pos);
            break;
          }
          case SDL_SCANCODE_TAB:
          {
            Json::StyledStreamWriter("  ").write(std::cout,
                                                 dumpJSON(this->server_));
            break;
          }
          default:
            break;
        }
        break;
      }
      case SDL_QUIT:
      {
        pong::crash("Exiting...");
        break;
      }
    }
  }

  void PaddleGameState::update()
  {
    this->server_.step();
  }

  void PaddleGameState::render(SDL_Renderer* renderer) const
  {
    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
    for(id_type id : this->server_.objects())
    {
      pong::render(renderer, this->server_.getObject(id).getVolume());
    }
}
}
