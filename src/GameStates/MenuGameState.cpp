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
 * \file MenuGameState.cpp
 * \brief File containing definitions for the MenuGameState
 */
#include "MenuGameState.h"
#include "render_text.h"
//#include "SinglePlayerGameState.h"
namespace pong
{
  MenuGameState::MenuGameState(Game& game) noexcept :
                                           title_("Ultimate Pong", 110),
                                           singleplayer_("Singleplayer"),
                                           multiplayer_("Multiplayer"),
                                           options_("Options"),
                                           help_("Help"),
                                           quit_("Quit"),
                                           game_(game)
  {
    this->title_.font_renderer(game.font_renderer());

    math::vector pos;
    pos.x = center(0, 1000, this->title_.getSurfaceWidth());
    pos.y = 40;
    this->title_.position(pos);

    std::size_t width = 550;
    std::size_t height = 75;
    pos.x = center(0, 1000, width);

    pos.y = 300;
    this->singleplayer_.position(pos);
    this->singleplayer_.width(width);
    this->singleplayer_.height(height);
    this->singleplayer_.enabled(false);
    this->singleplayer_.font_renderer(game.font_renderer());

    pos.y += 100;
    this->multiplayer_.position(pos);
    this->multiplayer_.width(width);
    this->multiplayer_.height(height);
    this->multiplayer_.enabled(false);
    this->multiplayer_.font_renderer(game.font_renderer());

    pos.y += 100;
    this->options_.position(pos);
    this->options_.width(width);
    this->options_.height(height);
    this->options_.enabled(false);
    this->options_.font_renderer(game.font_renderer());

    pos.y += 100;
    this->help_.position(pos);
    this->help_.width(width);
    this->help_.height(height);
    this->help_.enabled(false);
    this->help_.font_renderer(game.font_renderer());

    pos.y += 100;
    this->quit_.position(pos);
    this->quit_.width(width);
    this->quit_.height(height);
    this->quit_.font_renderer(game.font_renderer());

    this->addRenderableEntity(&this->title_);
    this->addRenderableEntity(&this->singleplayer_);
    this->addRenderableEntity(&this->multiplayer_);
    this->addRenderableEntity(&this->options_);
    this->addRenderableEntity(&this->help_);
    this->addRenderableEntity(&this->quit_);

#if 0
    this->singleplayer_.executeOnClick([]()
    {
      Game::getInstance()->setGameState(std::shared_ptr<GameState>(
                                                    new SinglePlayerGameState));
    });
#endif

    this->quit_.executeOnClick([this]()
    {
      this->game_.quit();
    });

    //Configure the mouse
    SDL_WM_GrabInput(SDL_GRAB_OFF);
    SDL_ShowCursor(SDL_ENABLE);
  }
  void MenuGameState::handleEvent(const SDL_Event& event)
  {
    if(event.type == SDL_KEYUP)
    {
      this->game_.quit();
    }

    this->singleplayer_.handleEvent(event);
    this->multiplayer_.handleEvent(event);
    this->options_.handleEvent(event);
    this->help_.handleEvent(event);
    this->quit_.handleEvent(event);
  }
};
