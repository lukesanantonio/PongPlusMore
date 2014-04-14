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
 *
 * \file serialize.hpp
 * \brief All the functions relating to serialization of objects to JSON.
 */
#include "serialize.h"
#include "Volume.h"
#include "server/Object.h"
#include "server/ObjectManager.h"
#include "server/Server.h"
#include <boost/lexical_cast.hpp>
namespace pong
{
  Json::Value dumpJSON(const Volume& v) noexcept
  {
    Json::Value root(Json::objectValue);
    Json::Value pos = dumpJSON(v.pos);

    root["Position"] = pos;
    root["Width"] = v.width;
    root["Height"] = v.height;

    return root;
  }

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

  Json::Value dumpJSON(const ObjectManager& objs) noexcept
  {
    Json::Value ar(Json::arrayValue);

    for(const auto& pair : objs)
    {
      Json::Value object = dumpJSON(std::get<1>(pair));
      object["Id"] = std::get<0>(pair);

      ar.append(object);
    }

    return ar;
  }
  Json::Value dumpJSON(const Node_Content& q) noexcept
  {
    Json::Value v(Json::objectValue);

    v["ObjectManager"] = boost::lexical_cast<std::string>(q.objs);
    v["Volume"] = dumpJSON(q.v);
    v["Max Objects"] = q.max_objs;
    v["Max Depth"] = q.max_level;
    v["Current Depth"] = q.current_level;

    using std::begin; using std::end;
    v["Ids"] = dumpJSON(begin(q.ids), end(q.ids));

    return v;
  }

  Volume getVolume(const Json::Value& root) noexcept
  {
    Volume vol;

    vol.height = root["Height"].asInt();
    vol.width = root["Width"].asInt();

    vol.pos.x = root["Position"]["x"].asDouble();
    vol.pos.y = root["Position"]["y"].asDouble();

    return vol;
  }
}
