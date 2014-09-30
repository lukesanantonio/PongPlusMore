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
#include <cstdint>
#include <memory>
#include "GameState.h"
#include <json/json.h>
#include "settings.h"
namespace pong
{
  struct Game
  {
    Game() = default;
    Game(const Game&) = delete;

    std::shared_ptr<GameState> game_state;
    bool exiting = false;

    bool slow = false;
    int start_count = 20;
    int count = 0;

    // debugging stats
    int fps = 0;

    ClientSettings settings;
  };
}
