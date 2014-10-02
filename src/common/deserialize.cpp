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
    o.physics_options = parse_physics(root["PhysicsOptions"]);
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

  DEFINE_PARSER(net::req::Request, json)
  {
    using parser_t = wrap_types_t<net::req::Request_Types,
                                  net::req::Request_Parser>;
    return parser_t::parse(json);
  }
}
