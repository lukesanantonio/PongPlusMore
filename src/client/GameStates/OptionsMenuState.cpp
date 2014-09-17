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
#include "OptionsMenuState.h"
#include "common/center.hpp"
#include "../render_text.h"
namespace pong
{
  OptionsMenuState::OptionsMenuState(Game& game) : g_(game)
  {
    Volume vol = {};
    vol.width = 750;
    vol.height = 100;
    vol.pos.x = center(0, game.settings.extent.x, vol.width);
    vol.pos.y = 375;

    render_debug_.volume(vol);
    render_debug_.font_renderer(game.settings.font.get());

    ButtonSwitch<bool> render_debug_switch(render_debug_, "Render Debug: ");

    render_debug_switch.on_change([&](bool on)
    {
      game.settings.render_debug = on;
    });

    render_debug_switch.add_switch("No", false);
    render_debug_switch.add_switch("Yes", true);

    render_debug_.onClick(render_debug_switch);
  }
  void OptionsMenuState::handleEvent(const SDL_Event& event)
  {
    if(event.type == SDL_QUIT)
    {
      g_.exiting = true;
    }
    render_debug_.handleEvent(event);
  }
  void OptionsMenuState::update() {}
  void OptionsMenuState::render(SDL_Renderer* renderer) const
  {
    render_debug_.render(renderer);
  }
}
