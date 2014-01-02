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
#include <utility>
#include "Server.h"
namespace pong
{
  struct ConcreteServer : public Server
  {
  public:
    using paddle_x_type = decltype(Paddle::pos.x);

    ConcreteServer(int16_t width, int16_t height) noexcept;
    virtual ~ConcreteServer() noexcept = default;

    virtual PaddleID makePaddle() override;

    virtual void
    setPaddleDestination(PaddleID, paddle_x_type) noexcept override;

    virtual std::vector<PaddleID> paddles() const noexcept override;
    virtual std::vector<BallID> balls() const noexcept override;

    virtual Paddle getPaddleFromID(PaddleID) const noexcept override;
    virtual Ball getBallFromID(BallID) const noexcept override;

    virtual void step() override;
  private:
    std::pair<Paddle, paddle_x_type> first_paddle_;
    std::pair<Paddle, paddle_x_type> second_paddle_;
    std::pair<Ball, decltype(Ball::pos) > ball_;

    /*!
     * \brief Used to implement makePaddle.
     */
    PaddleID id_impl_ = 1;
  };
}
