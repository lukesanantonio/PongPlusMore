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
    PhysicsOptions& physobj = obj.physics_options;
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
    PhysicsOptions& physopt = obj.physics_options;
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
    // Do this first so that if it fails we can avoid this *relatively*
    // hefty calculation.
    id_type id = this->quadtree_.insertObject(v, opt);
    if(!id) return id;
    auto containing_nodes = find_containing_nodes(this->quadtree_.root(), v);
    for(const Quadtree::node_type* n : containing_nodes)
    {
      for(id_type col_id : n->get_data()->ids)
      {
        if(col_id == id) continue;
        Volume col_v = n->get_data()->objs->findObject(col_id).volume;

        if(isIntersecting(v, col_v))
        {
          this->quadtree_.erase(id);
          return 0;
        }
      }
    }
    return id;
  }

  struct ModifiedObjectReference
  {
    Object obj;
    id_type id;
  };

  math::vector<double> find_opposing_forces(ModifiedObjectReference obj,
                                            Quadtree& q,
                                            int game_width, int game_height)
  {
    math::vector<double> forces = {0,0};
    Volume bounds = {{0, 0}, static_cast<double>(game_width),
                             static_cast<double>(game_height)};
    if(!isInsideVolume(obj.obj.volume, bounds))
    {
      std::vector<VolumeSide> vs = allProtrudingSides(obj.obj.volume, bounds);
      for(VolumeSide s : vs)
      {
        switch(s)
        {
          case VolumeSide::Top:
          {
            forces.y = 1;
            break;
          }
          case VolumeSide::Bottom:
          {
            forces.y = -1;
            break;
          }
          case VolumeSide::Left:
          {
            forces.x = 1;
            break;
          }
          case VolumeSide::Right:
          {
            forces.x = -1;
            break;
          }
          default: break;
        }
      }
    }
    return forces;
  }

  void raytrace(id_type id, Quadtree& q) noexcept
  {
    Object obj = q.findObject(id);

    if(obj.physics_options.type == PhysicsType::Ball)
    {
      math::vector<double>& velocity =
                                     obj.physics_options.ball_options.velocity;
      math::vector<double>& force = obj.physics_options.ball_options.force;

      if(force.x == 1.0) // We are being forced right.
      {
        velocity.x *= velocity.x < 0.0 ? -1 : 1;
      }
      else if(force.x == -1.0) // We are being forced left.
      {
        velocity.x *= 0.0 < velocity.x ? -1 : 1;
      }
      if(force.y == 1.0) // We are being forced down.
      {
        velocity.y *= velocity.y < 0.0 ? -1 : 1;
      }
      else if(force.y == -1.0) // We are being forced up.
      {
        velocity.y *= 0.0 < velocity.y ? -1 : 1;
      }

      obj.volume.pos += velocity;

      force = find_opposing_forces({obj, id}, q, 1000, 1000);
    }
    else if(obj.physics_options.type == PhysicsType::Paddle)
    {
      math::vector<double> original_position = obj.volume.pos;
      obj.volume.pos = obj.physics_options.paddle_options.destination;

      math::vector<double> movement{};

      movement = obj.volume.pos - original_position;
      if(movement.x > 0) movement.x = 1;
      if(movement.x < 0) movement.x = -1;
      if(movement.y > 0) movement.y = 1;
      if(movement.y < 0) movement.y = -1;


      math::vector<double> force = find_opposing_forces({obj, id}, q,
                                                        1000, 1000);
      // If we moved in a direction against our force. Go back
      if(force.x == -movement.x)
      {
        obj.volume.pos.x = original_position.x;
      }
      // Same here.
      if(force.y == -movement.y)
      {
        obj.volume.pos.y = original_position.y;
      }
    }

    q.setObject(id, obj);
  }

  void LocalServer::step() noexcept
  {
    for(id_type id : this->quadtree_.obj_manager().ids())
    {
      raytrace(id, this->quadtree_);
    }
  }
}
