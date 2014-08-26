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
#pragma once
#include "../Game.h"
#include "../GameState.h"
#include "server/LocalServer.h"
#include "common/serialize.h"
#include <iostream>
namespace pong
{
  struct PaddleGameState : public GameState
  {
  public:
    PaddleGameState(Game& g, Volume v) : g_(g), server_(v) {}
    virtual void handleEvent(const SDL_Event& event) override;
    virtual void update() override;
    virtual void render(SDL_Renderer*) const override;
  private:
    Game& g_;
    LocalServer server_;
    id_type top_ = 0;
    id_type bottom_ = 0;
    id_type ball_ = 0;
    bool render_quadtree_ = false;
    bool render_constraints_ = false;
  };
}
