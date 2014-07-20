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

        if(intersecting(v, col_v))
        {
          this->quadtree_.erase(id);
          return 0;
        }
      }
    }
    return id;
  }
#if 0
  template <typename point_type>
  math::vector<point_type> constrain(math::vector<point_type> v,
                                     VolumeSides constraints) noexcept
  {
    if(constraints & VolumeSide::Left)
      v.x = std::max(v.x, 0.0);
    if(constraints & VolumeSide::Right)
      v.x = std::min(v.x, 0.0);
    if(constraints & VolumeSide::Top)
      v.y = std::max(v.y, 0.0);
    if(constraints & VolumeSide::Bottom)
      v.y = std::min(v.y, 0.0);

    return v;
  }

  math::vector<double> get_displacement(Object obj) noexcept
  {
    math::vector<double> disp;
    if(isPaddle(obj))
    {
      disp = obj.physics_options.paddle_options.destination - obj.volume.pos;
    }
    else
    {
      disp = obj.physics_options.ball_options.velocity +
             obj.physics_options.ball_options.displacement_queue;
    }

    // Dilute our displacement factoring in our movement constraints.
    return constrain(disp, obj.physics_options.constraints);
  }


  struct ModifiedObjectReference
  {
    id_type id;
    Object obj;
  };
  void react(ModifiedObjectReference& obj, Quadtree& q) noexcept
  {
    obj.obj.physics_options.constraints = 0x00;

    if(isBall(obj.obj))
    {
      // Clear the displacement queue if it makes sense to do so.
      obj.obj.physics_options.ball_options.displacement_queue = {0,0};
    }

    Volume bounds = {{0,0}, 1000, 1000};
    VolumeSides sides = protruding_sides(obj.obj.volume, bounds);

    for(VolumeSides s : sides)
    {
      math::vector<double> d =
                       snapToVolumeInsideDiff(obj.obj.volume, s, bounds);
      obj.obj.volume.pos += d;
      if(isBall(obj.obj))
      {
        /*
        VolumeSide::Left:
        VolumeSide::Right:
        {
          obj.obj.physics_options.ball_options.velocity.x *= -1;
          break;
        }
        VolumeSide::Top:
        VolumeSide::Bottom:
        {
          obj.obj.physics_options.ball_options.velocity.y *= -1;
          break;
        }*/
      }
    }

    for(const auto& obj_pair : q.obj_manager())
    {
      if(obj_pair.first == obj.id) continue;

      Object& self = obj.obj;
      const Object& other = obj_pair.second;

      if(isIntersecting(other.volume, self.volume))
      {
        if(isPaddle(self) && isPaddle(other))
        {
          // We can't go there. Add a constraint for next time.
          self.physics_options.constraints |=
                                    findClosestSide(self.volume, other.volume);
        }
        else if(isBall(self))
        {
          if(isPaddle(other))
          {
            // We can't move, so flip our velocity, queue a return and add a
            // constraint.
            VolumeSides s = findClosestSide(self.volume, other.volume);
            self.physics_options.constraints |= s;
            switch(s)
            {
              case VolumeSide::Left:
              case VolumeSide::Right:
              {
                self.physics_options.ball_options.velocity.x *= -1;
                break;
              }
              case VolumeSide::Top:
              case VolumeSide::Bottom:
              {
                self.physics_options.ball_options.velocity.y *= -1;
                break;
              }
              default: break;
            }

            auto& q = self.physics_options.ball_options.displacement_queue;
            q += -snapDiff(other.volume, s, self.volume);
          }
          if(isBall(other))
          {
            using std::swap;
            Object other_obj_copy = other;

            // Swap velocities and add constraints.
            swap(self.physics_options.ball_options.velocity,
                 other_obj_copy.physics_options.ball_options.velocity);

            // Set the constraint of each.
            VolumeSides closest_side =
                                    findClosestSide(self.volume, other.volume);
            self.physics_options.constraints |= closest_side;
            other_obj_copy.physics_options.constraints |= closest_side;

            // Update the other obj.
            q.setObject(obj_pair.first, other_obj_copy);
          }
        }
      }
    }
  }

  void raytrace(id_type id, Quadtree& q) noexcept
  {
    ModifiedObjectReference obj = {id, q.findObject(id)};

    // Find our displacement.
    obj.obj.volume.pos += get_displacement(obj.obj);

    // Respond to any collisions and recalculate constraints.
    react(obj, q);

    // Commit that object to the quad-tree.
    q.setObject(obj.id, obj.obj);
  }

  void LocalServer::step() noexcept
  {
    std::vector<id_type> ids = this->quadtree_.obj_manager().ids();
    using std::begin; using std::end;
    std::sort(begin(ids), end(ids), [&](id_type i1, id_type i2)
    {
      return isBall(this->quadtree_.obj_manager(), i1);
    });

    for(id_type id : ids)
    {
      raytrace(id, this->quadtree_);
    }
  }
#endif
  void LocalServer::step() noexcept
  {
  }
}
