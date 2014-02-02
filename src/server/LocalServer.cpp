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
    if(++id_counter_ == 0x00) throw NoMorePaddlesAvailable{};
    this->world_.paddles.emplace_back(id_counter_, vol);
    return id_counter_;
  }
  id_type LocalServer::makeBall(const Volume& vol,
                                math::vector<int> vel)
  {
    if(++id_counter_ == 0x00) throw NoMoreBallsAvailable{};
    this->world_.paddles.emplace_back(id_counter_, vol);
    return id_counter_;
  }

  Object LocalServer::getObject(id_type id) const
  {
    using std::begin; using std::end;

    const auto& paddles = this->world_.paddles;
    const auto& balls = this->world_.balls;

    std::vector<Object> objs(paddles.size() + balls.size());

    auto iter_end = std::copy(begin(paddles), end(paddles), begin(objs));
    std::copy(begin(balls), end(balls), iter_end);

    auto iter = findObjectByID(begin(objs), end(objs), id);

    if(iter == end(objs)) throw InvalidID{};
    else return *iter;
  }
  Paddle LocalServer::getPaddle(id_type id) const
  {
    const auto& paddles = this->world_.paddles;
    using std::begin; using std::end;
    auto iter = findObjectByID(begin(paddles), end(paddles), id);
    if(iter == end(paddles)) throw InvalidID{};
    return *iter;
  }
  Ball LocalServer::getBall(id_type id) const
  {
    const auto& balls = this->world_.balls;
    using std::begin; using std::end;
    auto iter = findObjectByID(begin(balls), end(balls), id);
    if(iter == end(balls)) throw InvalidID{};
    return *iter;
  }

  bool LocalServer::isPaddle(id_type id) const
  {
    const auto& paddles = this->world_.paddles;
    using std::begin; using std::end;
    return findObjectByID(begin(paddles), end(paddles), id) != end(paddles);
  }
  bool LocalServer::isBall(id_type id) const
  {
    const auto& balls = this->world_.balls;
    using std::begin; using std::end;
    return findObjectByID(begin(balls), end(balls), id) != end(balls);
  }

  std::vector<id_type> LocalServer::paddles() const noexcept
  {
    const auto& paddles = this->world_.paddles;
    std::vector<id_type> ids(paddles.size());

    using std::begin; using std::end;
    std::transform(begin(paddles), end(paddles), begin(ids), id_of);

    return ids;
  }
  std::vector<id_type> LocalServer::balls() const noexcept
  {
    const auto& balls = this->world_.balls;
    std::vector<id_type> ids(balls.size());

    using std::begin; using std::end;
    std::transform(begin(balls), end(balls), begin(ids), id_of);

    return ids;
  }
  std::vector<id_type> LocalServer::objects() const noexcept
  {
    const auto& paddles = this->world_.paddles;
    const auto& balls = this->world_.balls;
    std::vector<id_type> ids(paddles.size() + balls.size());

    using std::begin; using std::end;
    auto new_end = std::transform(begin(paddles), end(paddles), begin(ids),
                                  id_of);
    std::transform(begin(balls), end(balls), new_end, id_of);

    return ids;
  }

  void LocalServer::step() noexcept
  {
    struct PhysicsObject
    {
      // Including an id means a find operation *every time.* A pointer will
      // be vastly more efficient.
      Object* object;
      math::vector<int> destination;
      bool getsPriority;
    };

    using std::begin; using std::end;

    std::vector<PhysicsObject> objs(world_.paddles.size() +
                                    world_.balls.size());

    // Build our PhysicsObjects from both our Paddles and balls.
    auto next = std::transform(begin(world_.paddles), end(world_.paddles),
                               begin(objs),
    [](Paddle& obj) -> PhysicsObject
    {
      return {&obj, math::normalize<long double>(obj.getNextPosition()) * 7,
              true};
    });

    std::transform(begin(world_.balls), end(world_.balls), begin(objs),
    [](Ball& ball) -> PhysicsObject
    {
      return {&ball, ball.getVelocity() + ball.getVolume().pos, false};
    });
  }
}
