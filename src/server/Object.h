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
#include "common/Volume.h"
#include <json/json.h>
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
  };
  struct BallOptions
  {
    math::vector<double> velocity;
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
                                  : type(type) {}

    PhysicsType type;
    union
    {
      PaddleOptions paddle_options;
      BallOptions ball_options;
    };
  };

  using id_type = uint16_t;
  struct Object
  {
    virtual ~Object() noexcept {}

    explicit Object(const Volume& vol = Volume{},
                    PhysicsOptions opts = {})
                    : vol_(vol), physics_options_(opts) {}

    const Volume& getVolume() const noexcept { return this->vol_; }
    Volume& getVolume() noexcept { return this->vol_; }

    const PhysicsOptions& getPhysicsOptions() const noexcept
    { return this->physics_options_; }

    PhysicsOptions& getPhysicsOptions() noexcept
    { return this->physics_options_; }

  private:
    Volume vol_;
    PhysicsOptions physics_options_;
  };

  Json::Value dumpJSON(const PhysicsOptions&) noexcept;
  Json::Value dumpJSON(const Object&) noexcept;
}
