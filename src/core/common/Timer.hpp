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
#include <chrono>
namespace pong
{
  template <class ClockType = std::chrono::high_resolution_clock>
  class Timer
  {
  public:
    template <class DurationType>
    bool hasBeen(DurationType duration) const;

    template <class DurationType>
    DurationType hasBeen() const;

    void reset();
  private:
    std::chrono::time_point<ClockType> time_before_ = ClockType::now();
  };

  template <class ClockType>
  template <class DurationType>
  bool Timer<ClockType>::hasBeen(DurationType duration) const
  {
    return ClockType::now() - this->time_before_ >= duration;
  }

  template <class ClockType>
  template <class DurationType>
  DurationType Timer<ClockType>::hasBeen() const
  {
    using std::chrono::duration_cast;
    return duration_cast<DurationType>(ClockType::now() - this->time_before_);
  }

  template <class ClockType>
  void Timer<ClockType>::reset()
  {
    this->time_before_ = ClockType::now();
  }
}
