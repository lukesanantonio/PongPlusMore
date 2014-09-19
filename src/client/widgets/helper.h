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
#include <memory>
#include "Button.h"
#include "../Game.h"
#include "../GameState.h"
#include "common/vector.h"
namespace pong
{
  inline Button back_button(Game& g,
                            std::shared_ptr<GameState> prev_state) noexcept
  {
    Button b;
    b.text("Back");
    b.on_click([=, &g]()
    {
      g.game_state = prev_state;
    });
    b.add_hotkey(SDL_SCANCODE_ESCAPE);
    b.font_renderer(g.settings.font.get());

    constexpr static int from_side = 35;
    Volume v = {{}, 300, 75};
    v.pos.x = from_side;
    v.pos.y = g.settings.extent.y - v.height - from_side;
    b.volume(v);

    return b;
  }
}
