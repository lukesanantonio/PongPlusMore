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
#include "Object.h"
namespace pong
{
  Json::Value dumpJSON(const PhysicsOptions& phys) noexcept
  {
    Json::Value root(Json::objectValue);

    if(phys.type == PhysicsType::Paddle)
    {
      root["Destination"] = dumpJSON(phys.paddle_options.destination);
      root["Max Velocity"] = phys.paddle_options.max_velocity;
    }
    if(phys.type == PhysicsType::Ball)
    {
      root["Velocity"] = dumpJSON(phys.ball_options.velocity);
    }

    return root;
  }
  Json::Value dumpJSON(const Object& obj) noexcept
  {
    Json::Value root(Json::objectValue);
    Json::Value vol = dumpJSON(obj.getVolume());
    Json::Value phys = dumpJSON(obj.getPhysicsOptions());

    root["Volume"] = vol;
    root["PhysicsOptions"] = phys;

    return root;
  }
}
