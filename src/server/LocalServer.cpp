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

  void stepObject(id_type id, ObjectManager& obj_manager) noexcept
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

      // Move one or less unit towards our destination. Easy.
      diff = math::normalize(dp_temp) * std::min(math::length(dp_temp), 1.0);
    }
    else if(isBall(obj))
    {
      // Easy.
      diff = obj.getPhysicsOptions().ball_options.velocity;
    }

    // Keep the original.
    Object original = obj;

    // Move our object
    obj.getVolume().pos += diff;

    // Check collision.
    auto intersecting = findIntersectingObjects(id, obj_manager);

    using std::begin; using std::end;
    // Move the paddles to the front.
    std::partition(begin(intersecting), end(intersecting),
     std::bind(static_cast<bool (*)(const ObjectManager&, id_type)>(&isPaddle),
               std::ref(obj_manager), std::placeholders::_1));

    for(id_type other_id : intersecting)
    {
      Object& other_obj = obj_manager.findObject(other_id);

      if(isPaddle(obj) && isPaddle(other_obj))
      {
        // The other object was here first as a paddle. So we shouldn't be
        // here.
        obj = original;
        break;
      }
      else if((isPaddle(obj) && isBall(other_obj)) ||
              (isBall(obj) && isPaddle(other_obj)))
      {
        // Paddle-Ball collision, this is where it gets interesting.
        Object& paddle = isPaddle(obj) ? obj : other_obj;
        Object& ball   = isBall(obj)   ? obj : other_obj;

        VolumeSide s = findClosestSide(paddle.getVolume(), ball.getVolume());
        switch(s)
        {
          case VolumeSide::Left:
          case VolumeSide::Right:
          {
            ball.getPhysicsOptions().ball_options.velocity.x *= -1;
            break;
          }
          case VolumeSide::Top:
          case VolumeSide::Bottom:
          {
            ball.getPhysicsOptions().ball_options.velocity.y *= -1;
            break;
          }
          default: break;
        }

        snapVolumeToVolume(ball.getVolume(), s, paddle.getVolume());
      }
      else if(isBall(obj) && isBall(other_obj))
      {
        // Switch velocities.
        obj = original;

        using std::swap;
        swap(obj.getPhysicsOptions().ball_options.velocity,
             other_obj.getPhysicsOptions().ball_options.velocity);

        // But wait, obj hasn't actually been moved at this point.
        // Just step it in the *right* direction now.
        stepObject(id, obj_manager);
      }
      else
      {
        // WTF?
        // TODO: Scripting.
      }
    }
  }
  void LocalServer::step() noexcept
  {
    for(std::pair<const id_type, Object>& obj_pair : this->objs_)
    {
      stepObject(std::get<0>(obj_pair), this->objs_);
    }
  }
}
