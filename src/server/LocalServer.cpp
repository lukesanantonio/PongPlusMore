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
#include <algorithm>
#include <functional>
namespace pong
{
  id_type LocalServer::makePaddle(const Volume& vol)
  {
    // If ++id_counter_ is 0, then we already used up every id.
    if(++id_counter_ == 0x00) throw OutOfIDs{};
    this->world_.objs.emplace_back(id_counter_, vol, PhysicsType::Paddle);
    return id_counter_;
  }
  id_type LocalServer::makeBall(const Volume& vol,
                                math::vector<int> vel)
  {
    if(++id_counter_ == 0x00) throw OutOfIDs{};
    this->world_.objs.emplace_back(id_counter_, vol, PhysicsType::Ball);
    return id_counter_;
  }

  Object LocalServer::getObject(id_type id) const
  {
    using std::begin; using std::end;

    auto iter = findObjectByID(begin(world_.objs), end(world_.objs), id);

    if(iter == end(world_.objs)) throw InvalidID{};
    else return *iter;
  }

  bool LocalServer::isPaddle(id_type id) const
  {
    try
    {
      return
      this->getObject(id).getPhysicsOptions().type == PhysicsType::Paddle;
    }
    catch(...)
    {
      return false;
    }
  }
  bool LocalServer::isBall(id_type id) const
  {
    try
    {
      return
      this->getObject(id).getPhysicsOptions().type == PhysicsType::Ball;
    }
    catch(...)
    {
      return false;
    }
  }

  std::vector<id_type> LocalServer::objects() const noexcept
  {
    std::vector<id_type> ids(world_.objs.size());

    using std::begin; using std::end;
    std::transform(begin(world_.objs), end(world_.objs), begin(ids), id_of);

    return ids;
  }

  void LocalServer::step() noexcept
  {
  }
}
