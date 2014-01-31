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
#include "common/Volume.h"
#include "util.h"
namespace pong
{
  struct Ball
  {
  public:
    explicit Ball(id_type id = 0,
                  Volume vol = Volume{},
                  math::vector<int> vel = math::vector<int>())
             : id_(id), vol_(vol), vel_(vel) {}
  private:
    const id_type id_;
    Volume vol_;
    math::vector<int> vel_;
  public:
    inline Volume& getVolume() noexcept;
    inline const Volume& getVolume() const noexcept;

    const math::vector<int>& getVelocity() const noexcept {return this->vel_;}
    math::vector<int>& getVelocity() noexcept { return this->vel_; }

    id_type id() const noexcept { return this->id_; }
  };

  inline Volume& Ball::getVolume() noexcept
  {
    return this->vol_;
  }
  inline const Volume& Ball::getVolume() const noexcept
  {
    return this->vol_;
  }
}
