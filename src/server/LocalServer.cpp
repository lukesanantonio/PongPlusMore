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
    std::vector<id_type> ids(this->objs_.size());

    using std::begin; using std::end;
    std::transform(begin(this->objs_), end(this->objs_), begin(ids),
    [](const auto& pair) -> id_type
    {
      return pair.first;
    });

    return ids;
  }

  void moveObject(id_type id, ObjectManager& obj_manager) noexcept;

  bool moveObject(id_type id, ObjectManager& obj_manager,
                  math::vector<double> diff, bool slave=false) noexcept
  {
    Object& obj = obj_manager.findObject(id);

    Object original = obj;
    obj.getVolume().pos += diff;

    auto intersecting = findIntersectingObjects(id, obj_manager);

    using std::begin; using std::end;
    std::partition(begin(intersecting), end(intersecting),
     std::bind(static_cast<bool (*)(const ObjectManager&, id_type)>(&isPaddle),
               std::ref(obj_manager), std::placeholders::_1));

    for(id_type other_id : intersecting)
    {
      Object& other_obj = obj_manager.findObject(other_id);
      Object other_original = other_obj;
      if(isPaddle(obj) && isPaddle(other_obj))
      {
        // It is unacceptable for a paddle to collide with a paddle.
        // If this is what we were asked, the answer is no.
        if(slave)
        {
          // Don't change any state!
          obj = original;
          return false;
        }

        obj = original;

        // We *cannot* move to diff.
        // But! Can we use either component to maneuver around this obstacle?
        math::vector<double> only_x = diff;
        only_x.y = 0;
        math::vector<double> only_y = diff;
        only_y.x = 0;

        // Two calls are required so that infinite recursion doesn't occur.
        if(moveObject(id, obj_manager, only_x, true))
        {
          // Well, we can move using the x component
          // However that means different collisions need to be dealt with.
          moveObject(id, obj_manager, only_x, false);
          return true;
        }
        if(moveObject(id, obj_manager, only_y, true))
        {
          moveObject(id, obj_manager, only_y, false);
          return true;
        }

        // Well, there is a paddle still there even after trying both comps.
        return false;
      }
    }

    // Assume no collisions. That's a success.
    if(slave) obj = original;
    return true;
  }

  void moveObject(id_type id, ObjectManager& obj_manager) noexcept
  {
    Object& obj = obj_manager.findObject(id);

    // Our change in position this iteration.
    math::vector<double> diff;

    // If we have a paddle on our hands.
    // TODO: Put this code in a function, it will help scripting custom object
    // types also.
    if(isPaddle(obj))
    {
      //                   .- Delta position
      math::vector<double> dp_temp =
                           obj.getPhysicsOptions().paddle_options.destination -
                           obj.getVolume().pos;

      // Move one or less units towards our destination. Easy.
      diff = math::normalize(dp_temp) * std::min(math::length(dp_temp), 1.0);
    }
    else if(isBall(obj))
    {
      // Easy.
      diff = obj.getPhysicsOptions().ball_options.velocity;
    }

    moveObject(id, obj_manager, diff);
  }
  void LocalServer::step() noexcept
  {
    std::vector<id_type> to_delete;
    Volume bounds{{0,0}, 1000, 1000};
    for(std::pair<const id_type, Object>& obj_pair : this->objs_)
    {
      if(!isIntersecting(std::get<1>(obj_pair).getVolume(), bounds))
      {
        to_delete.push_back(std::get<0>(obj_pair));
      }
    }

    using fun_type =
          ObjectManager::size_type (ObjectManager::*)(ObjectManager::key_type);
    fun_type fun= &ObjectManager::erase;

    using std::begin; using std::end;
    std::for_each(begin(to_delete), end(to_delete),
                  std::bind(fun, std::ref(this->objs_),
                            std::placeholders::_1));
    for(std::pair<const id_type, Object>& obj_pair : this->objs_)
    {
      moveObject(std::get<0>(obj_pair), this->objs_);
    }
  }
}
