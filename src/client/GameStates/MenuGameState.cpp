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
#include "MenuGameState.h"
#include "common/crash.hpp"
#include "common/center.hpp"
#include "PaddleGameState.h"
#include "OptionsMenuState.h"
#include "MultiplayerSelectState.h"

namespace pong
{
  MenuGameState::MenuGameState(Game& game)
                             : title_("Pong Plus More", 120, {0,0}),
                               singleplayer_("Singleplayer"),
                               multiplayer_("Multiplayer"),
                               options_("Options"),
                               quit_("Quit")
  {
    FontRenderer* font_renderer = game.settings.font.get();
    this->title_.font_renderer(font_renderer);

    this->title_.position({center(0, game.settings.extent.x,
                                  this->title_.surface_width()),
                           50});

    int width = 750;
    int height = 90;
    int position_x = center(0, game.settings.extent.x, width);
    int starting_y = 280;

    for(Button* button : {&singleplayer_, &multiplayer_, &options_, &quit_})
    {
      button->volume({math::vector<double>(position_x, starting_y),
                     static_cast<double>(width), static_cast<double>(height)});
      button->font_renderer(font_renderer);
      button->enabled(false);
      button->background_color({0xff, 0xff, 0xff, 0xff});
      button->text_color({0x00, 0x00, 0x00, 0xff});
      starting_y += height + 50;
    }

    this->singleplayer_.enabled(true);
    this->singleplayer_.on_click([&]()
    {
      game.game_state = std::make_shared<PaddleGameState>(game);
    });

    this->multiplayer_.enabled(true);
    this->multiplayer_.on_click([&]()
    {
      game.game_state =
               std::make_shared<MultiplayerSelectState>(game, game.game_state);
    });

    this->options_.enabled(true);
    this->options_.on_click([&]()
    {
      game.game_state = std::make_shared<OptionsMenuState>(game,
                                                           game.game_state);
    });

    this->quit_.enabled(true);
    this->quit_.add_hotkey(SDL_SCANCODE_ESCAPE);
    this->quit_.on_click([&]()
    {
      game.exiting = true;
    });
  }

  void MenuGameState::handleEvent(const SDL_Event& event)
  {
    this->singleplayer_.handleEvent(event);
    this->multiplayer_.handleEvent(event);
    this->options_.handleEvent(event);
    this->quit_.handleEvent(event);
  }

  void MenuGameState::update()
  {
  }

  void MenuGameState::render(SDL_Renderer* renderer) const
  {
    this->title_.render(renderer);
    this->singleplayer_.render(renderer);
    this->multiplayer_.render(renderer);
    this->options_.render(renderer);
    this->quit_.render(renderer);
  }
}
