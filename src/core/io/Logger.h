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
#include <cstdio>
#include <string>
#include <uv.h>
#include "../common/pif/helper.h"
namespace pong
{
  enum class Severity
  {
    Debug = 0,
    Info,
    Warning,
    Error,
    Unspecified
  };

  enum class Logger_Stream
  {
    Out, Err, None
  };

  struct Logger
  {
    Logger(Logger_Stream = Logger_Stream::Out) noexcept;
    Logger(std::string const&) noexcept;
   ~Logger() noexcept;

    void log(Severity s, const std::string& msg) noexcept;
    void step() noexcept;

    inline int fd() const noexcept;

    inline void min_severity(Severity s) noexcept;
    inline Severity min_severity() const noexcept;
  private:
    int fd_;
    bool should_close_;

    Severity min_severity_;

    uv_loop_t* loop_;
  };

  inline int Logger::fd() const noexcept
  {
    return fd_;
  }

  inline void Logger::min_severity(Severity s) noexcept
  {
    min_severity_ = s;
  }
  inline Severity Logger::min_severity() const noexcept
  {
    return min_severity_;
  }
}
BEGIN_FORMATTER_SCOPE
{
  BEGIN_DECLARE_FORMATTABLE_ENUM(pong::Severity, int, 5)
  ENUM_VALUE(pong::Severity::Debug, "debug"),
  ENUM_VALUE(pong::Severity::Info, "info"),
  ENUM_VALUE(pong::Severity::Warning, "warning"),
  ENUM_VALUE(pong::Severity::Error, "error"),
  ENUM_VALUE(pong::Severity::Unspecified, "unspecified")
  END_DECLARE_FORMATTABLE_ENUM(pong::Severity)
}
END_FORMATTER_SCOPE
