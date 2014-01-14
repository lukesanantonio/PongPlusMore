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
#include <algorithm>
#include "ConcreteServer.h"
#include "common/center.hpp"
namespace pong
{
  ConcreteServer::ConcreteServer(int width, int height) noexcept :
                     ball_(1, { width / 2, height / 2 }, {0, 0}, 20)
  {}

  PaddleID ConcreteServer::makePaddle()
  {
    if(this->paddles_.size() >= 2)
    {
      return 0;
    }

    this->paddles_.emplace_back(Paddle(this->paddles_.size(), {0, 0}, 200, 75),
                                0);
    return std::get<0>(this->paddles_.back()).id;
  }
  void ConcreteServer::setPaddleDestination(PaddleID id,
                                            paddle_x_type x) noexcept
  {
    for(auto& pair : this->paddles_)
    {
      if(std::get<0>(pair).id == id)
      {
        std::get<1>(pair) = x;
      }
    }
  }
  std::vector<PaddleID> ConcreteServer::paddles() const noexcept
  {
    //This function is fairly inefficient. TODO fix.
    std::vector<PaddleID> paddles(this->paddles_.size());
    std::transform(this->paddles_.begin(), this->paddles_.end(),
                   paddles.begin(),
    [](const std::pair<Paddle, int>& pair)
    {
      return std::get<0>(pair).id;
    });

    return paddles;
  }
  std::vector<BallID> ConcreteServer::balls() const noexcept
  {
    return {this->ball_.id};
  }
  Paddle ConcreteServer::getPaddleFromID(PaddleID id) const noexcept
  {
    return std::get<0>(this->paddles_[id - 1]);
  }
  Ball ConcreteServer::getBallFromID(BallID id) const noexcept
  {
    switch(id)
    {
      case 1:
        return this->ball_;
      default:
        return Ball();
    }
  }
};
