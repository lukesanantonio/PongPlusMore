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
#include "log.h"
#include <iostream>
#include <ctime>
#include <mutex>
namespace pong
{
  void log(severity s, const std::string& msg) noexcept
  {
    static std::mutex m;
    std::lock_guard<std::mutex> lg(m);

    // Stringify severity
    std::string severity;
    if(s == severity::info) severity = "info";
    else if(s == severity::warning) severity = "warning";
    else if(s == severity::error) severity = "error";
    else severity = "unspecified";

    // Stringify current time.
    std::string time;
    time.resize(20);

    std::time_t t = std::time(NULL);
    std::size_t count = std::strftime(&time[0], 20, "%F|%T", std::gmtime(&t));
    if(count == 0)
    {
      time = "badtime";
    }

    time.resize(count);

    std::clog << "(" << time << "): " << severity << ": " << msg << std::endl;
  }
}
