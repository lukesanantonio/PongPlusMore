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
#include <cstdint>
#include "common/vector.h"
#include "Volume.h"
namespace pong
{
  /*!
   * \brief Used to identify a paddle to the server.
   *
   * \note A PaddleID of zero does not reference any paddle but remains
   * semantically correct.
   */
  using PaddleID = uint8_t;
  struct Paddle
  {
  public:
    using id_type = PaddleID;

    explicit Paddle(PaddleID id = 0,
                    math::vector<int> pos = math::vector<int>(),
                    int width = 0,
                    int height = 0) : id_(id), volume_{pos, width, height}{}
  private:
    const PaddleID id_;
    /*!
     * \brief The hoped to be location.
     *
     * The physics will simulate the paddle until it gets here.
     */
    math::vector<int> next_pos_;
    Volume volume_;
  public:
    inline Volume getVolume() const noexcept { return this->volume_; }

    inline PaddleID id() const noexcept { return this->id_; }

    inline const math::vector<int>& getPosition() const noexcept
    {
      return this->volume_.pos;
    }
    inline math::vector<int>& getPosition() noexcept
    {
      return this->volume_.pos;
    }

    inline const math::vector<int>& getNextPosition() const noexcept
    {
      return this->next_pos_;
    }
    inline math::vector<int>& getNextPosition() noexcept
    {
      return this->next_pos_;
    }

    inline int getWidth() const noexcept { return this->volume_.width; }
    inline int& getWidth() noexcept { return this->volume_.width; }

    inline int getHeight() const noexcept { return this->volume_.height; }
    inline int& getHeight() noexcept { return this->volume_.height; }

  };
}
