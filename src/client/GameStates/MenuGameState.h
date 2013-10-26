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
 * \file MenuGameState.h
 * \brief File containing declarations of the MenuGameState.
 */
#pragma once
#include "GameState.h"
#include "Label.h"
#include "Button.h"
#include "Game.h"
namespace pong
{
  /*!
   * \brief First game state shown, the menu.
   */
  class MenuGameState : public GameState
  {
  public:
    MenuGameState(Game& game) noexcept;
    ~MenuGameState() noexcept = default;
    MenuGameState(const MenuGameState&) = delete;
    MenuGameState(MenuGameState&&) = delete;
    MenuGameState& operator=(const MenuGameState&) = delete;
    MenuGameState& operator=(MenuGameState&&) = delete;

    virtual void update() override {}
    virtual void handleEvent(const SDL_Event& event) override;
  private:
    Label title_;

    Button singleplayer_;
    Button multiplayer_;
    Button options_;
    Button help_;
    Button quit_;

    Game& game_;
  };
};
