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
#include "input_impls.h"
#include "SDL/SDL.h"
namespace pong
{
  double MouseInput::get_position() const noexcept
  {
    int x;
    SDL_GetMouseState(&x, NULL);
    return x;
  }
  double TestingAI::get_position() const noexcept
  {
    const Object& self = this->objs_.find_object(this->id_);

    // Find the first ball.
    for(id_type id : this->objs_.ids())
    {
      const Object& obj = this->objs_.find_object(id);
      if(isBall(this->objs_.find_object(id)))
      {
        this->last_x_ = obj.volume.pos.x - self.volume.width / 2;
        return this->last_x_;
      }
    }
    return this->last_x_;
  }
}
