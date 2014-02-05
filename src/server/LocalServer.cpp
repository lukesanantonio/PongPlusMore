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
namespace pong
{
  void LocalServer::setDestination(id_type id, math::vector<double> dest)
  {
    // Might throw an exception, fine let it throw!
    PhysicsOptions& physobj = this->objs_.findObject(id).getPhysicsOptions();
    if(physobj.type != PhysicsType::Paddle)
    {
      physobj.type = PhysicsType::Paddle;
      physobj.paddle_options = PaddleOptions{};
    }
    physobj.paddle_options.destination = dest;
  }
  void LocalServer::setVelocity(id_type id, math::vector<double> vel)
  {
    PhysicsOptions& physopt = this->objs_.findObject(id).getPhysicsOptions();
    if(physopt.type != PhysicsType::Ball)
    {
      physopt.type = PhysicsType::Ball;
      physopt.ball_options = BallOptions{};
    }
    physopt.ball_options.velocity = vel;
  }

  Object LocalServer::getObject(id_type id) const
  {
    return this->objs_.findObject(id);
  }
  std::vector<id_type> LocalServer::objects() const noexcept
  {
    std::vector<id_type> ids;

    using std::begin; using std::end;
    std::transform(begin(this->objs_), end(this->objs_), begin(ids),
    [](const auto& pair) -> id_type
    {
      return pair.first;
    });

    return ids;
  }

  void LocalServer::step() noexcept
  {
  }
}
