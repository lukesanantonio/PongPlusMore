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
#include "exceptions.h"
#include "collision_util.h"
namespace pong
{
  PaddleID LocalServer::connect()
  {
    // We've done a loop, no more ids.
    if(++id_counter_ == 0x00) throw NoMoreClientsAvailable();
    // TODO Add support for configurable dimensions of the Paddle.
    this->world_.paddles.emplace_back(id_counter_,
                                      math::vector<int>(0, 0), 250, 40);
    return id_counter_;
  }
  Paddle& LocalServer::getPaddle(PaddleID id)
  {
    return findPaddleByID(this->world_, id);
  }
  const Paddle& LocalServer::getPaddle(PaddleID id) const
  {
    return findPaddleByID(this->world_, id);
  }
  Ball& LocalServer::getBall(BallID id)
  {
    return findBallByID(this->world_, id);
  }
  const Ball& LocalServer::getBall(BallID id) const
  {
    return findBallByID(this->world_, id);
  }

  std::vector<PaddleID> LocalServer::paddles() const noexcept
  {
    const std::vector<Paddle>& paddles = this->world_.paddles;
    std::vector<PaddleID> ids(paddles.size());

    std::transform(paddles.begin(), paddles.end(), ids.begin(),
                   [&](const Paddle& paddle){ return paddle.id(); });
    return ids;
  }

  std::vector<BallID> LocalServer::balls() const noexcept
  {
    const std::vector<Ball>& balls = this->world_.balls;
    std::vector<BallID> ids(balls.size());

    std::transform(balls.begin(), balls.end(), ids.begin(),
                   [&](const Ball& ball){ return ball.id(); });
    return ids;
  }

  void LocalServer::step() noexcept
  {
    for(Paddle& paddle : this->world_.paddles)
    {
      math::vector<int>& pos = paddle.getPosition();
      math::vector<int>& next_pos = paddle.getNextPosition();
      math::vector<int> diff = {next_pos.x - pos.x, next_pos.y - pos.y};

      // We don't want to move to fast.
      // This method adds that terminal-velocity effect that is kind of
      // interesting.
      double new_length = std::min<double>(math::length<double>(diff), 5);

      std::vector<math::vector<int> > points =
                          raytrace(math::normalize<double>(diff) * new_length);
      // Add our current position to the points.
      // That way we can modify pos in place, without losing anything, since
      // its original location is the first element of the vector anyway.
      std::transform(points.begin(), points.end(), points.begin(),
                     [&](const math::vector<int>& point){return point + pos;});

      // The first point is 0,0. TODO add some sort of assertion of that.
      for(auto iter = points.begin() + 1; iter != points.end(); ++iter)
      {
        bool can_move = true;
        for(PaddleID other_id : this->paddles())
        {
          if(other_id == paddle.id()) continue;
          Volume vol = paddle.getVolume();
          vol.pos = *iter;
          if(isIntersecting(vol, this->getPaddle(other_id).getVolume()))
          {
            can_move = false;
            break;
          }
        }
        if(can_move) // Nothing is obstructing us from moving to *iter.
        {
          pos = *(iter);
        }
      }
    }
  }
}
