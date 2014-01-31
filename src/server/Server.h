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
#include "common/vector.h"
#include "Object.h"
#include "Paddle.h"
#include "Ball.h"
#include "util.h"
namespace pong
{
  struct Server
  {
    virtual ~Server() noexcept = default;

    /*!
     * \brief Spawns a new paddle with a volume.
     *
     * \param vol The volume *requested* of the paddle.
     *
     * \returns The id of the created paddle.
     */
    virtual id_type makePaddle(const Volume& vol) = 0;
    /*!
     * \brief Spawns a new ball with a volume and a velocity.
     *
     * \param vol The volume *requested* of the ball.
     * \param vel The velocity of the ball.
     *
     * \returns The id of the created ball.
     */
    virtual id_type makeBall(const Volume& vol, math::vector<int> vel) = 0;

    virtual Object getObject(id_type) const = 0;
    virtual Paddle getPaddle(id_type) const = 0;
    virtual Ball getBall(id_type) const = 0;

    virtual bool isPaddle(id_type) const = 0;
    virtual bool isBall(id_type) const = 0;

    virtual std::vector<id_type> paddles() const noexcept = 0;
    virtual std::vector<id_type> balls() const noexcept = 0;
    virtual std::vector<id_type> objects() const noexcept = 0;

    virtual void step() noexcept = 0;
  };
}
