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
#pragma once
#include "core/common/volume.h"
#include "core/common/dump/helper.h"
#include "core/common/parse/helper.h"
namespace pong
{
  enum class PhysicsType
  {
    Undefined,
    Paddle,
    Ball
  };
  struct PaddleOptions
  {
    math::vector<double> destination;

    DECLARE_PARSER_TYPE(parse::Object<PaddleOptions>);
    DECLARE_DUMPER_TYPE(dump::Object<PaddleOptions>);

    DECLARE_PROPERTY_VALUES(1, "Destination");
    DECLARE_PROPERTIES_TUPLE(math::vector<double>);
    DECLARE_PROPERTIES(this->destination);
  };
  struct BallOptions
  {
    math::vector<double> velocity;

    DECLARE_PARSER_TYPE(parse::Object<BallOptions>);
    DECLARE_DUMPER_TYPE(dump::Object<PaddleOptions>);

    DECLARE_PROPERTY_VALUES(1, "Velocity");
    DECLARE_PROPERTIES_TUPLE(math::vector<double>);
    DECLARE_PROPERTIES(this->velocity);
  };

  /*!
   * \brief A variant to encapsulate the differences of a paddle and a ball.
   *
   * Kind of works like the SDL_Event structure. This kind of thing is elegant
   * *enough*.
   */
  struct PhysicsOptions
  {
    /* implicit */ PhysicsOptions(PhysicsType type = PhysicsType::Undefined)
                                  : type(type)
    {
      switch(type)
      {
        case PhysicsType::Paddle:
        {
          this->paddle_options = PaddleOptions();
          break;
        }
        case PhysicsType::Ball:
        {
          this->ball_options = BallOptions();
          break;
        }
        default: break;
      }
    }

    PhysicsType type;
    union
    {
      PaddleOptions paddle_options;
      BallOptions ball_options;
    };
    VolumeSides constraints = VolumeSide::None;
  };

  using id_type = uint16_t;
  struct Object
  {
    explicit Object(const Volume& vol = Volume{},
                    PhysicsOptions opts = {})
                    : volume(vol), physics_options(opts) {}

    Volume volume;
    PhysicsOptions physics_options;

    DECLARE_PARSER_TYPE(parse::Object<Object>);
    DECLARE_DUMPER_TYPE(dump::Object<Object>);

    DECLARE_PROPERTY_VALUES(2, "Volume", "PhysicsOptions");
    DECLARE_PROPERTIES_TUPLE(Volume, PhysicsOptions);
    DECLARE_PROPERTIES(this->volume, this->physics_options);
  };

  inline Object make_paddle(const Volume& v) noexcept
  {
    return Object{v, PhysicsType::Paddle};
  }
  inline Object make_ball(const Volume& v) noexcept
  {
    return Object{v, PhysicsType::Ball};
  }

  inline bool isPaddle(const Object& obj) noexcept
  {
    return obj.physics_options.type == PhysicsType::Paddle;
  }
  inline bool isBall(const Object& obj) noexcept
  {
    return obj.physics_options.type == PhysicsType::Ball;
  }
}
