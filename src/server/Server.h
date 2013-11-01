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
#include <vector>
#include "Paddle.h"
#include "Ball.h"
namespace pong
{
  struct Server
  {
    virtual ~Server() noexcept = default;
    /*!
     * \brief A new paddle is added. The id is returned.
     *
     * \returns A non-zero integer which is the id of the paddle just paired.
     * \sa PaddleID
     */
    virtual PaddleID makePaddle() = 0;

    /*!
     * \brief Requests a paddle x location for a given PaddleID.
     *
     * The request won't be fulfilled until a call to Server::step, so it can
     * be changed at any time before a step.
     * \note Should the paddle id be 0, this function is a no-op.
     */
    virtual void setPaddleDestination(PaddleID,
                                      decltype(Paddle::pos.x) x) noexcept = 0;

    /*!
     * \brief Returns a vector of all the paddle ids that exist.
     */
    virtual std::vector<PaddleID> paddles() const noexcept = 0;
    /*!
     * \brief Returns a vector of all the balls ids that exist.
     */
    virtual std::vector<BallID> balls() const noexcept = 0;

    /*!
     * \brief Returns the paddle with the id of the one passed in.
     */
    virtual Paddle getPaddleFromID(PaddleID) const noexcept = 0;
    /*!
     * \brief Returns the ball with the id of the one passed in.
     */
    virtual Ball getBallFromID(BallID) const noexcept = 0;

    /*!
     * \brief Steps the pong simulation.
     *
     * \post All requested paddle locations are fulfilled.
     */
    virtual void step() = 0;
  };
}
