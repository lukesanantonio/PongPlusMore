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
#define DEFINE_PARSER(type, vname)\
type parser<type>::parse(Json::Value const& vname) noexcept

namespace pong
{
  DEFINE_PARSER(int, json)
  {
    return json.asInt();
  }
  DEFINE_PARSER(std::string, json)
  {
    return json.asString();
  }
  DEFINE_PARSER(math::vector<double>, json)
  {
    math::vector<double> vec;
    vec.x = json["x"].asInt();
    vec.y = json["y"].asInt();
    return vec;
  }
  DEFINE_PARSER(Volume, root)
  {
    Volume v;

    v.pos = parse_vector(root["Position"]);
    v.width = root["Width"].asDouble();
    v.height = root["Height"].asDouble();

    return v;
  }
  DEFINE_PARSER(Object, root)
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
      opt.paddle_options.destination = parse_vector(phys_json["Destination"]);
    }

    return o;
  }

  DEFINE_PARSER(Severity, val)
  {
    std::string s = val.asString();
    if(s == "info") return Severity::Info;
    if(s == "warning") return Severity::Warning;
    if(s == "error") return Severity::Error;
    return Severity::Unspecified;
  }
}

#undef DEFINE_PARSER
