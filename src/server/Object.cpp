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
  DEFINE_PROPERTY_VALUES(PaddleOptions);
  DEFINE_PROPERTY_VALUES(BallOptions);

  DEFINE_PROPERTY_VALUES(Object);
}

BEGIN_FORMATTER_SCOPE
{
  DEFINE_PARSER(pong::PhysicsOptions, phys_json)
  {
    using pong::PhysicsOptions;
    using pong::PhysicsType;
    using pong::PaddleOptions;
    using pong::BallOptions;

    PhysicsOptions opt;
    if(phys_json["Destination"].isObject())
    {
      opt.type = PhysicsType::Paddle;
      opt.paddle_options = find_formatter_t<PaddleOptions>::parse(phys_json);
    }
    else if(phys_json["Velocity"].isObject())
    {
      opt.type = PhysicsType::Ball;
      opt.ball_options = find_formatter_t<BallOptions>::parse(phys_json);
    }
    return opt;
  }
  DEFINE_DUMPER(pong::PhysicsOptions, phys)
  {
    using pong::PhysicsOptions;
    using pong::PhysicsType;
    using pong::PaddleOptions;
    using pong::BallOptions;

    Json::Value json;

    if(phys.type == PhysicsType::Paddle)
    {
      using paddle_options_formatter_t = find_formatter_t<PaddleOptions>;
      json["Destination"] =
                         paddle_options_formatter_t::dump(phys.paddle_options);
    }
    else if(phys.type == PhysicsType::Ball)
    {
      using ball_options_formatter_t = find_formatter_t<BallOptions>;
      json["Velocity"] = ball_options_formatter_t::dump(phys.ball_options);
    }

    return json;
  }
}
END_FORMATTER_SCOPE
