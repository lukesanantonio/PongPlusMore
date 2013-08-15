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
 * \file PaddleController.hpp
 * \brief Contains the declaration of the ABC for controlling a paddle.
 */
#pragma once
#include "SDL.h"
#include "EventHandler.h"
namespace pong
{
  class Paddle;
  /*!
   * \brief ABC for controlling a paddle. Yay, an abstraction.
   */
  struct PaddleController : public EventHandler
  {
  public:
    PaddleController() = default;
    virtual ~PaddleController() = default;
    PaddleController(const PaddleController&) = delete;
    PaddleController(PaddleController&&) = delete;
    PaddleController& operator=(const PaddleController&) = delete;
    PaddleController& operator=(PaddleController&&) = delete;

    /*!
     * \brief Notify the controller of an event.
     *
     * This will most likely be used to maintain the current state of some
     * elements, such as where the mouse is at any given moment!
     */
    virtual void handleEvent(const SDL_Event&) noexcept override = 0;

    /*!
     * \brief Generates a position and updates the Paddle accordingly.
     *
     * \param paddle The paddle which needs an updated position. Will be
     * modified!
     */
    virtual void update(Paddle* paddle) = 0;
  };
};
