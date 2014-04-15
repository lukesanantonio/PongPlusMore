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
#include "collision_util.h"
namespace pong
{
  void LocalServer::setDestination(id_type id, math::vector<double> dest)
  {
    // Might throw an exception, fine let it throw!
    Object obj = this->quadtree_.findObject(id);
    PhysicsOptions& physobj = obj.getPhysicsOptions();
    if(physobj.type != PhysicsType::Paddle)
    {
      physobj.type = PhysicsType::Paddle;
      physobj.paddle_options = PaddleOptions{};
    }
    physobj.paddle_options.destination = dest;

    this->quadtree_.setObject(id, obj);
  }
  void LocalServer::setVelocity(id_type id, math::vector<double> vel)
  {
    Object obj = this->quadtree_.findObject(id);
    PhysicsOptions& physopt = obj.getPhysicsOptions();
    if(physopt.type != PhysicsType::Ball)
    {
      physopt.type = PhysicsType::Ball;
      physopt.ball_options = BallOptions{};
    }
    physopt.ball_options.velocity = vel;

    this->quadtree_.setObject(id, obj);
  }

  Object LocalServer::getObject(id_type id) const
  {
    return this->quadtree_.findObject(id);
  }
  std::vector<id_type> LocalServer::objects() const noexcept
  {
    return this->quadtree_.obj_manager().ids();
  }

  /*!
   * \brief Inserts an object into the Quadtree.
   *
   * \note If the object will intersect with any other object the new one *is
   * not* inserted.
   */
  id_type LocalServer::insertObject(const Volume& v,
                                    const PhysicsOptions& opt) noexcept
  {
    id_type id = this->quadtree_.insertObject(v, opt);
    if(!id) return 0;
    auto containing_nodes = find_containing_nodes(this->quadtree_.root(), id);
    for(const Quadtree::node_type* n : containing_nodes)
    {
      for(id_type col_id : n->get_data()->ids)
      {
        if(col_id == id) continue;
        Volume col_v = n->get_data()->objs->findObject(col_id).getVolume();

        if(isIntersecting(v, col_v))
        {
          this->quadtree_.erase(id);
          return 0;
        }
      }
    }
    return id;
  }

  void LocalServer::step() noexcept
  {
    for(id_type id : this->quadtree_.obj_manager().ids())
    {
      // For every object that needs to move.
      Object obj = this->quadtree_.findObject(id);

      switch(obj.getPhysicsOptions().type)
      {
        case PhysicsType::Paddle:
        {
          obj.getVolume().pos =
                           obj.getPhysicsOptions().paddle_options.destination;
          break;
        }
        case PhysicsType::Ball:
        {
          obj.getVolume().pos += obj.getPhysicsOptions().ball_options.velocity;
          break;
        }
        default: break;
      };

      this->quadtree_.setObject(id, obj);
    }
  }
}
