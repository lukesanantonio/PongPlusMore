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
#include "deserialize.h"
namespace pong
{
  math::vector<double> parse_vector(const Json::Value& root) noexcept
  {
    math::vector<double> v;
    v.x = root["x"].asDouble();
    v.y = root["y"].asDouble();
    return v;
  }
  Volume parse_volume(const Json::Value& root) noexcept
  {
    Volume v;

    v.pos = parse_vector(root["Position"]);
    v.width = root["Width"].asDouble();
    v.height = root["Height"].asDouble();

    return v;
  }
  Object parse_object(const Json::Value& root) noexcept
  {
    Object o;
    o.volume = parse_volume(root["Volume"]);

    PhysicsOptions& opt = o.physics_options;

    Json::Value phys_json = root["PhysicsOptions"];
    if(phys_json["Velocity"].isObject())
    {
      opt.type = PhysicsType::Ball;
      opt.ball_options = BallOptions();
      opt.ball_options.velocity = parse_vector(phys_json["Velocity"]);
    }
    else if(phys_json["Destination"].isObject())
    {
      opt.type = PhysicsType::Paddle;
      opt.paddle_options = PaddleOptions();
      opt.paddle_options.destination = parse_vector(phys_json["Desination"]);
    }

    return o;
  }

  Severity parse_severity(const Json::Value& val) noexcept
  {
    std::string s = val.asString();
    if(s == "info") return Severity::Info;
    if(s == "warning") return Severity::Warning;
    if(s == "error") return Severity::Error;
    return Severity::Unspecified;
  }

  ObjectCreationAction parse_create_action(const Json::Value& params) noexcept
  {
    ObjectCreationAction a;
    a.obj = parse_object(params[0]);
    return a;
  }

  ObjectDeletionAction parse_delete_action(const Json::Value& params) noexcept
  {
    ObjectDeletionAction a;
    a.id = params[0].asInt();
    return a;
  }
  LogAction parse_log_action(const Json::Value& params) noexcept
  {
    LogAction a;
    a.severity = parse_severity(params[0].asString());
    a.msg = params[1].asString();
    return a;
  }
}
