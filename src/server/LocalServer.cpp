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
#include "LocalServer.h"
#include "exceptions.h"
namespace pong
{
  PaddleID LocalServer::connect()
  {
    // We've done a loop, no more ids.
    if(++id_counter_ == 0x00) throw NoMoreClientsAvailable();
    this->world_.paddles.emplace_back(id_counter_);
    return id_counter_;
  }
  Paddle& LocalServer::getPaddle(PaddleID id)
  {
    return findPaddleByID(this->world_, id);
  }
  const Paddle& LocalServer::getPaddle(PaddleID id) const
  {
    return findPaddleByID(this->world_, id);
  }
}
