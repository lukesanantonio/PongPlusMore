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
#include "Server.h"
#include "World.h"
namespace pong
{
  struct LocalServer : public Server
  {
    virtual PaddleID connect() override;

    virtual Paddle& getPaddle(PaddleID) override;
    virtual const Paddle& getPaddle(PaddleID) const override;

    virtual Ball& getBall(BallID) override;
    virtual const Ball& getBall(BallID) const override;

    virtual std::vector<PaddleID> paddles() const noexcept override;
    virtual std::vector<BallID> balls() const noexcept override;
  private:
    World world_;
    PaddleID id_counter_ = 0;
  };
}
