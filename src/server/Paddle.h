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
#include "Object.h"
namespace pong
{
  struct Paddle : public Object
  {
  public:
    explicit Paddle(id_type id = 0, const Volume& vol = Volume{},
                    math::vector<int> next_pos = math::vector<int>())
                    : Object{id, vol}, next_pos_(next_pos){}
  private:
    /*!
     * \brief The hoped to be location.
     *
     * The physics will simulate the paddle until it gets here.
     */
    math::vector<int> next_pos_;
  public:
    inline const math::vector<int>& getNextPosition() const noexcept
    {
      return this->next_pos_;
    }
    inline math::vector<int>& getNextPosition() noexcept
    {
      return this->next_pos_;
    }
  };
}
