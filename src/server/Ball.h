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
#include "common/vector.h"
#include "Volume.h"
namespace pong
{
  using BallID = uint8_t;
  struct Ball
  {
  public:
    using id_type = BallID;

    explicit Ball(BallID id = 0,
                  int diameter = 0,
                  math::vector<int> pos = math::vector<int>(),
                  math::vector<int> vel = math::vector<int>())
             : id_(id), pos_(pos), diameter_(diameter){}
  private:
    const BallID id_;
    int diameter_;
    math::vector<int> pos_;
    math::vector<int> vel_;
  public:
    inline Volume getVolume() const noexcept;

    BallID id() const noexcept { return this->id_; }

    const math::vector<int>& getPosition() const noexcept {return this->pos_;}
    math::vector<int>& getPosition() noexcept { return this->pos_; }

    const math::vector<int>& getVelocity() const noexcept {return this->vel_;}
    math::vector<int>& getVelocity() noexcept { return this->vel_; }

    const int& diameter() const noexcept { return this->diameter_; }
    int& diameter() noexcept { return this->diameter_; }
  };

  inline Volume Ball::getVolume() const noexcept
  {
    Volume vol;

    vol.pos.x = this->pos_.x;
    vol.pos.y = this->pos_.y;
    vol.width = this->diameter_;
    vol.height= this->diameter_;

    return vol;
  }
}
