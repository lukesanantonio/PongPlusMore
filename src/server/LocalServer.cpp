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

#if 0
  bool moveObject(id_type id, ObjectManager& obj_manager,
                  bool slave) noexcept;

  /*!
   * \param slave Implies no change in the provided object is wanted/needed.
   */
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
        obj = original;

        // It is unacceptable for a paddle to collide with a paddle.
        // If this is what we were asked, the answer is no.
        if(slave)
        {
          // Don't change any state!
          return false;
        }

        // if !slave:

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
      if((isPaddle(obj) && isBall(other_obj)) ||
         (isBall(obj) && isPaddle(other_obj)))
      {
        bool this_object_is_paddle = isPaddle(obj);

        // If we were only asked if we could move here and we are just a ball.
        // well sorry, we can't. Restore than return false.
        if(slave && !this_object_is_paddle)
        {
          // The user wanted to move a ball.
          // Into a paddle?
          // Hell no.

          // the ball
          obj = original;
          return false;
        }

        Object& paddle = isPaddle(obj) ? obj : other_obj;
        Object& ball   = isBall(obj)   ? obj : other_obj;

        id_type paddle_id = isPaddle(obj) ? id : other_id;
        id_type ball_id = isBall(obj) ? id : other_id;

        // Handle this collision no matter what.
        VolumeSide s = findClosestSide(paddle.getVolume(), ball.getVolume());
        math::vector<double>& ball_velocity =
                                ball.getPhysicsOptions().ball_options.velocity;

        using std::abs;
        switch(s)
        {
          case VolumeSide::Right:
          {
            ball_velocity.x = abs(ball_velocity.x);
            break;
          }
          case VolumeSide::Left:
          {
            ball_velocity.x = -abs(ball_velocity.x);
            break;
          }
          case VolumeSide::Top:
          {
            ball_velocity.y = -abs(ball_velocity.y);
            break;
          }
          case VolumeSide::Bottom:
          {
            ball_velocity.y = abs(ball_velocity.y);
            break;
          }
          default: break;
        }

        if(!this_object_is_paddle)
        {
          // Restore volume.
          ball.getVolume().pos = original.getVolume().pos;
          // Bad move.
          return false;
        }

        // Well, we only have the option to move the ball, when the paddle
        // collided with the ball. If the ball was moving along minding its
        // own business and hit a paddle, bouncing off is enough.
        // Only move if paddle.
        if(this_object_is_paddle)
        {
          // So a paddle moved where a ball was. We react (change velocity)
          // then move the ball out of the way.
          math::vector<double> ball_diff = snapDiff(ball.getVolume(),
                                                    s, paddle.getVolume());
          // Can we move the ball?
          if(moveObject(ball_id, obj_manager, ball_diff, true))
          {
            // Nice. Move it. This is where the recursive ball-to-ball
            // collision handling happens.
            moveObject(ball_id, obj_manager, ball_diff, false);
          }
          else
          {
            // If the ball can't move for some reason. Well that means the
            // paddle can't me moved either. Abort collisions.
            obj = original;
            return false;
          }
        }
      }
      if(isBall(obj) && isBall(other_obj))
      {
        // We most likely can move.
        // Make sure we can move the other ball out of the way.
        VolumeSide s = findClosestSide(obj.getVolume(),
                                       other_obj.getVolume());
        math::vector<double> ball_diff = snapDiff(other_obj.getVolume(), s,
                                                  obj.getVolume());

        if(slave)
        {
          // If we are asked, well it all matters if we can move the ball
          // ahead of us.
          // However if it isn't false, some other collision could be false
          // so we can't commit to saying we *can* by returning true.
          if(!moveObject(other_id, obj_manager, ball_diff, true))
          {
            // Aw well, restore.
            // TODO I really need to figure out a better way to restore the
            // object.
            obj = original;
            return false;
          }
          continue;
        }

        // If we ain't slaven' we can swap velocities and in other words
        // modify the object.
        using std::swap;
        swap(obj.getPhysicsOptions().ball_options.velocity,
             other_obj.getPhysicsOptions().ball_options.velocity);
        // The velocities swapped will be restored anyway if there's a problem
        // so we good.

        if(moveObject(other_id, obj_manager, ball_diff, true))
        {
          moveObject(other_id, obj_manager, ball_diff, false);
        }
        else
        {
          // Oh god, well we need to go back because we obviously can't
          // stay here.
          obj = original;
          other_obj = other_original;
          return false;
        }
      }
    }

    // But wait, wall?
    Volume bounds{{0,0}, 1000, 1000};

    if(!isInsideVolume(bounds, obj.getVolume()))
    {
      // We simply cannot move there.
      if(isPaddle(obj))
      {
        obj = original;
        return false;
      }

      // if isBall(obj):

      // We are probably intersecting a wall. But which one?
      VolumeSide s = closestSideFromInside(bounds, obj.getVolume());

      // This is absolutely copied from line 157.
      // That's horrible
      math::vector<double>& ball_velocity =
                                 obj.getPhysicsOptions().ball_options.velocity;

      switch(s)
      {
        case VolumeSide::Right:
        {
          ball_velocity.x = -std::abs(ball_velocity.x);
          break;
        }
        case VolumeSide::Left:
        {
          ball_velocity.x = std::abs(ball_velocity.x);
          break;
        }
        case VolumeSide::Top:
        {
          ball_velocity.y = std::abs(ball_velocity.y);
          break;
        }
        case VolumeSide::Bottom:
        {
          ball_velocity.y = -std::abs(ball_velocity.y);
          break;
        }
        default: break;
      }

      // Now go back to where we were.
      obj.getVolume().pos = original.getVolume().pos;

      // Yes this is a valid move, but it won't actually end up moving the
      // ball, if we are in slave mode, return false so that things don't
      // think other things can be moved into a wall. But if we are actually
      // moving then it doesn't matter.
      return false;
    }

    // Assume no collisions. That's a success.
    if(slave) obj = original;
    return true;
  }

  bool moveObject(id_type id, ObjectManager& obj_manager,
                  bool slave = false) noexcept
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
      diff = math::normalize(dp_temp) *
             std::min(math::length(dp_temp),
                      obj.getPhysicsOptions().paddle_options.max_velocity);
    }
    else if(isBall(obj))
    {
      // Easy.
      diff = obj.getPhysicsOptions().ball_options.velocity;
    }

    return moveObject(id, obj_manager, diff, slave);
  }
#endif
  void LocalServer::step() noexcept
  {
#if 0
    for(std::pair<const id_type, Object>& obj_pair : this->objs_)
    {
      moveObject(std::get<0>(obj_pair), this->objs_);
    }
#endif
  }
}
