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
#include "log.h"
#include <string>
#include <ctime>
#include <cstring>
#include <uv.h>
#include "thread_local.h"
namespace ug
{
  Scoped_Log_Init::Scoped_Log_Init() noexcept
  {
    init_log();
  }
  Scoped_Log_Init::~Scoped_Log_Init() noexcept
  {
    uninit_log();
  }

  UG_THREAD_LOCAL uv_loop_t* loop_ = nullptr;
  Log_Severity out_level_ = Log_Severity::Info;

  void init_log() noexcept
  {
    if(loop_) return;

    loop_ = new uv_loop_t;
    uv_loop_init(loop_);
  }
  void uninit_log() noexcept
  {
    if(!loop_) return;

    uv_loop_close(loop_);
    delete loop_;
  }
  void flush_log() noexcept
  {
    uv_run(loop_, UV_RUN_NOWAIT);
  }
  void flush_log_full() noexcept
  {
    uv_run(loop_, UV_RUN_DEFAULT);
  }

  void set_out_log_level(Log_Severity level) noexcept
  {
    // TODO: Add a mutex or something so we don't get that 1-in-1000000 data
    // race. TODO On second thought if this ever comes up in practice by a
    // lotto ticket!
    out_level_ = level;
  }

  std::string format_time(const std::string& format)
  {
    // Stringify current time.
    std::string time;
    time.resize(25);

    std::time_t t = std::time(NULL);
    std::size_t count = std::strftime(&time[0], time.size(),
                                      format.c_str(), std::localtime(&t));
    if(count == 0)
    {
      time = "badtime";
    }

    time.resize(count);
    return time;
  }

  struct write_req_t
  {
    uv_fs_t req;
    uv_buf_t buf;
  };

  void after_log(uv_fs_t* fs_req)
  {
    write_req_t* req = (write_req_t*) fs_req;

    // Uninitialize actual uv request.
    uv_fs_req_cleanup(&req->req);

    // Deallocate buffer.
    delete[] req->buf.base;

    // Deallocate request subclass.
    delete req;
  }

  void log(std::string severity, std::string msg,
           char const* const before, char const* const after,
           int severity_level) noexcept
  {
    if(!loop_) return;

    // Bail out if the severity level isn't enough
    if(severity_level > (int) out_level_)
    {
      // Get outta here!
      return;
    }

    // Create the final message.
    std::string time = format_time("%F|%T");
    std::string final_msg = std::string{before} + "(" + time + "): " +
                            severity + ": " + msg + std::string{after} + "\n";

    // Copy to a buffer libuv can use.
    char* msg_data = new char[final_msg.size()];
    std::memcpy(msg_data, final_msg.data(), final_msg.size());

    // Make the request.
    write_req_t* req = new write_req_t;
    req->buf = uv_buf_init(msg_data, final_msg.size());
    uv_fs_write(loop_, &req->req, 1, &req->buf, 1, -1, after_log);
  }
  void log(Log_Severity severity, std::string msg) noexcept
  {
    // This could be implemented differently, for instance with a
    // enum-to-string function + calling the log(string,string) function
    // directly.
    switch(severity)
    {
      case Log_Severity::Error:
        log_e(msg);
        break;
      case Log_Severity::Warning:
        log_w(msg);
        break;
      case Log_Severity::Info:
        log_i(msg);
        break;
      case Log_Severity::Debug:
        log_d(msg);
    }
  }

  constexpr char const* const RESET_C = "\x1b[0m";
  constexpr char const* const ERROR_C = "\x1b[95m";
  constexpr char const* const WARNING_C = "\x1b[91m";
  constexpr char const* const INFO_C = "\x1b[92m";
  constexpr char const* const DEBUG_C = "\x1b[96m";

  void log_e(std::string msg) noexcept
  {
    log("error", msg, ERROR_C, RESET_C, static_cast<int>(Log_Severity::Error));
  }
  void log_w(std::string msg) noexcept
  {
    log("warning", msg, WARNING_C, RESET_C, static_cast<int>(Log_Severity::Warning));
  }
  void log_i(std::string msg) noexcept
  {
    log("info", msg, INFO_C, RESET_C, static_cast<int>(Log_Severity::Info));
  }
  void log_d(std::string msg) noexcept
  {
    log("debug", msg, DEBUG_C, RESET_C, static_cast<int>(Log_Severity::Debug));
  }
}
BEGIN_FORMATTER_SCOPE
{
  DEFINE_FORMATTABLE_ENUM(ug::Log_Severity)
}
END_FORMATTER_SCOPE
