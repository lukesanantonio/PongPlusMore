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

  /*!
   * \brief Finds the force upon object obj in its current position.
   *
   * TODO The use of a ModifiedObjectReference is mostly an implementation
   * detail, figure out a good interface to represent what we want to
   * represent.
   */
  math::vector<double> find_force(const ModifiedObjectReference& obj,
                                  Quadtree&,
                                  double game_width,
                                  double game_height) noexcept
  {
    // TODO implement paddle forces, somehow?
    if(obj.obj.physics_options.type != PhysicsType::Ball) return {};

    math::vector<double> force;
    Volume bounds = {{0,0}, game_width, game_height};
    VolumeSide side = mostProtrudingSide(bounds, obj.obj.volume);
    if(side != VolumeSide::None)
    {
      switch(side)
      {
        case VolumeSide::Top:
        {
          force.y = -obj.obj.physics_options.ball_options.velocity.y * 2;
          break;
        }
        case VolumeSide::Bottom:
        {
          force.y = -obj.obj.physics_options.ball_options.velocity.y * 2;
          break;
        }
        case VolumeSide::Left:
        {
          force.x = -obj.obj.physics_options.ball_options.velocity.x * 2;
          break;
        }
        case VolumeSide::Right:
        {
          force.x = -obj.obj.physics_options.ball_options.velocity.x * 2;
          break;
        }
      }
    }
    return force;
  }

  void raytrace_ball(id_type ball, Quadtree& q) noexcept
  {
    if(!isBall(q.obj_manager(), ball))
    {
      return;
    }

    Object ball_obj = q.findObject(ball);
    math::vector<double> starting_position = ball_obj.volume.pos;
    math::vector<double> velocity =
                            ball_obj.physics_options.ball_options.velocity;
    math::vector<double> end_position = starting_position + velocity;

    // Divide by the smallest amount that will give us less-than-.25 chunks.
    const int chunks = std::ceil(math::length(velocity) / .25);
    const double magnitude_per_chunk = math::length(velocity) / chunks;

    for(int chunk = 0; chunk < chunks; ++chunk)
    {
      // Calculate the new location.
      // The magnitude of the change is always magnitude_per_step unless that
      // distance would move the ball past its allowed distance to travel.

      ball_obj.volume.pos += magnitude_per_chunk * math::normalize(velocity);

      math::vector<double> force = find_force({ball_obj, ball}, q, 1000, 1000);

      if(force.x == 0.0 && force.y == 0.0) continue;

      ball_obj.physics_options.ball_options.velocity += force;
      velocity =
            math::normalize(ball_obj.physics_options.ball_options.velocity)
            * (math::length(velocity) - (chunks * magnitude_per_chunk));
      starting_position = ball_obj.volume.pos;
      end_position = ball_obj.volume.pos + velocity;
    }

    q.setObject(ball, ball_obj);
  }

  void LocalServer::step() noexcept
  {
    for(id_type id : this->quadtree_.obj_manager().ids())
    {
      // For every object that needs to move.
      Object obj = this->quadtree_.findObject(id);

      switch(obj.physics_options.type)
      {
        case PhysicsType::Paddle:
        {
          obj.volume.pos = obj.physics_options.paddle_options.destination;
          this->quadtree_.setObject(id, obj);
          break;
        }
        case PhysicsType::Ball:
        {
          raytrace_ball(id, this->quadtree_);
          break;
        }
        default: break;
      };
    }
  }
}
