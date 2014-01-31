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
#include "util.h"
#include "collision_util.h"
namespace pong
{
  id_type LocalServer::makePaddle(const Volume& vol)
  {
    // If ++id_counter_ is 0, then we already used up every id.
    if(++id_counter_ == 0x00) throw NoMorePaddlesAvailable{};
    this->world_.paddles.emplace_back(id_counter_, vol);
    return id_counter_;
  }
  id_type LocalServer::makeBall(const Volume& vol,
                                math::vector<int> vel)
  {
    if(++id_counter_ == 0x00) throw NoMoreBallsAvailable{};
    this->world_.paddles.emplace_back(id_counter_, vol);
    return id_counter_;
  }

  Object LocalServer::getObject(id_type) const {}
  Paddle LocalServer::getPaddle(id_type) const {}
  Ball LocalServer::getBall(id_type) const {}

  bool LocalServer::isPaddle(id_type) const {}
  bool LocalServer::isBall(id_type) const {}

  std::vector<id_type> LocalServer::paddles() const noexcept {}
  std::vector<id_type> LocalServer::balls() const noexcept {}

  void LocalServer::step() noexcept {}
}
