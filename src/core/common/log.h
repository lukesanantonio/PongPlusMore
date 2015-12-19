/*
 * uGlue - Glue many languages together into a whole with ukernel-inspired RPC.
 * Copyright (C) 2015 Luke San Antonio
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
#include <string>
#include "translate.h"
#include "pif/helper.h"

// We make this a macro so we can just not do it should it become something we
// don't want to be doing for some reason. Performance or IO bottleneck for
// example.
#ifndef PONG_DISABLE_LOGGING
  #define PONG_LOG_ATTEMPT_INIT() init_log()

  #define PONG_LOG_E(...) pong::log_e(__VA_ARGS__)
  #define PONG_LOG_W(...) pong::log_w(__VA_ARGS__)
  #define PONG_LOG_I(...) pong::log_i(__VA_ARGS__)
  #define PONG_LOG_D(...) pong::log_d(__VA_ARGS__)
#else
  #define PONG_LOG_ATTEMPT_INIT() ((void*)0)

  #define PONG_LOG_E ((void*)0)
  #define PONG_LOG_W ((void*)0)
  #define PONG_LOG_I ((void*)0)
  #define PONG_LOG_D ((void*)0)
#endif

namespace pong
{
  struct Scoped_Log_Init
  {
    Scoped_Log_Init() noexcept;
    ~Scoped_Log_Init() noexcept;
  };
  void init_log() noexcept;
  void uninit_log() noexcept;
  void flush_log() noexcept;
  void flush_log_full() noexcept;

  enum class Log_Severity : unsigned int
  {
    Debug = 0, Info, Warning, Error
  };

  void set_log_filename(char const* const filename) noexcept;
  void set_out_log_level(Log_Severity level) noexcept;

  template <class... Args>
  void log(Log_Severity severity, std::string msg, Args&&... args) noexcept
  {
    log(severity, format(msg, 0, "", std::forward<Args>(args)...));
  }
  void log(Log_Severity severity, std::string msg) noexcept;

  void log_e(std::string msg) noexcept;

  template <class... Args>
  void log_e(std::string msg, Args&&... args) noexcept
  {
    log_e(format(msg, 0, "", std::forward<Args>(args)...));
  }

  void log_w(std::string msg) noexcept;

  template <class... Args>
  void log_w(std::string msg, Args&&... args) noexcept
  {
    log_w(format(msg, 0, "", std::forward<Args>(args)...));
  }

  void log_i(std::string msg) noexcept;

  template <class... Args>
  void log_i(std::string msg, Args&&... args) noexcept
  {
    log_i(format(msg, 0, "", std::forward<Args>(args)...));
  }

  void log_d(std::string msg) noexcept;

  template <class... Args>
  void log_d(std::string msg, Args&&... args) noexcept
  {
    log_d(format(msg, 0, "", std::forward<Args>(args)...));
  }
}
BEGIN_FORMATTER_SCOPE
{
  BEGIN_DECLARE_FORMATTABLE_ENUM(pong::Log_Severity, unsigned int, 4)
    ENUM_VALUE(0, "debug"),
    ENUM_VALUE(1, "info"),
    ENUM_VALUE(2, "warning"),
    ENUM_VALUE(3, "error")
  END_DECLARE_FORMATTABLE_ENUM(pong::Log_Severity)
}
END_FORMATTER_SCOPE
