/*
 * uGlue - Glue many languages together into a whole with ukernel-inspired RPC.
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
  template <class Clock_Type = std::chrono::high_resolution_clock>
  class Timer
  {
  public:
    template <class DurationType>
    bool has_been(DurationType duration) const;

    template <class DurationType>
    DurationType has_been() const;

    void reset();
  private:
    std::chrono::time_point<Clock_Type> time_before_ = Clock_Type::now();
  };

  template <class Clock_Type>
  template <class Duration_Type>
  bool Timer<Clock_Type>::has_been(Duration_Type duration) const
  {
    return Clock_Type::now() - this->time_before_ >= duration;
  }

  template <class Clock_Type>
  template <class Duration_Type>
  Duration_Type Timer<Clock_Type>::has_been() const
  {
    using std::chrono::duration_cast;
    return duration_cast<Duration_Type>(Clock_Type::now()-this->time_before_);
  }

  template <class Clock_Type>
  void Timer<Clock_Type>::reset()
  {
    this->time_before_ = Clock_Type::now();
  }
}
