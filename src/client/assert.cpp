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
 * \file common.cpp
 * \brief Definitions to miscellaneous functions which anyone might find useful.
 */
#include "common.h"
#include <iostream>
#include <cstdlib>
#include <csignal>
namespace pong
{
  /*!
   * \brief Performs a runtime assertion of a surface.
   *
   * Should the test fail (when !surface) pong::crash is called with a message
   * containing the surface width and height!
   *
   * \param surface The surface to perform a runtime assertion on.
   * \param width The requested width during initialization...
   * \param height The requested height during initialization...
   */
  void runtime_assert_surface(SDL_Surface* surface, int width, int height)
  {
    if(surface) return;

    std::string message = "Failed to create SDL_Surface* with...";

    message += "\n\twidth: " + std::to_string(width);
    message += "\n\theight: " + std::to_string(height);

    crash(message);
  }

  /*!
   * \brief Performs a runtime assertion of a surface.
   *
   * Should the test fail, (when !surface) pong::crash is called with a message
   * saying some SDL_Surface* failed to be created!
   */
  void runtime_assert_surface(SDL_Surface* surface)
  {
    if(surface) return;

    crash("Failed to create SDL_Surface*!");
  }
};
