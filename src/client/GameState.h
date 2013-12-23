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
 * \file GameState.h
 * \brief GameState ABC.
 */
#pragma once
#include "SDL.h"
#include <vector>
namespace pong
{
  class GameState
  {
  public:
    /*!
     * \brief Notifies the GameState of an event.
     *
     * A more specific kind of update function.
     */
    virtual void handleEvent(const SDL_Event& event) = 0;

    /*!
     * \brief Allows the GameState to update periodically and not necessarily
     * based on events.
     *
     * It is (should be) called with a 1:1 ratio to how GameState::render is
     * called. That means for every render call there is an update call
     * immediately preceding it!
     */
    virtual void update() = 0;

    virtual void render(SDL_Renderer*) const = 0;
  };
};
