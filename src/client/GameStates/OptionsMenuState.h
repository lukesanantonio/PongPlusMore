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
#include <array>
#include "../GameState.h"
#include "../Button.h"
#include "../Game.h"
#include "../ButtonSwitch.h"

namespace pong
{
  struct OptionsMenuState : public GameState
  {
    OptionsMenuState(Game& game, std::shared_ptr<GameState> return_state);
    virtual void handleEvent(const SDL_Event& event) override;
    virtual void update() override;
    virtual void render(SDL_Renderer* renderer) const override;
  private:
    Game& g_;
    std::shared_ptr<GameState> back_state_;
    Button render_debug_;
    Button back_;
  };
}
