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
#include "Logger.h"
#include <ctime>
namespace pong
{
  Logger::Logger() noexcept
  {
    this->loop_ = uv_loop_new();
  }
  Logger::~Logger() noexcept
  {
    uv_run(this->loop_, UV_RUN_DEFAULT);
    uv_loop_delete(this->loop_);
  }

  std::string stringify_severity(const Severity s)
  {
    std::string severity;
    if(s == Severity::Info) severity = "Info";
    else if(s == Severity::Warning) severity = "Warning";
    else if(s == Severity::Error) severity = "Error";
    else severity = "Unspecified";
    return severity;
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

  void after_write(uv_fs_t* fs_req)
  {
    write_req_t* req = (write_req_t*) fs_req;

    // Uninitialize actual uv request.
    uv_fs_req_cleanup(&req->req);

    // Deallocate buffer.
    delete[] req->buf.base;

    // Deallocate request subclass.
    delete req;
  }

  void Logger::log(Severity s, const std::string& msg) noexcept
  {
    // Stringify severity
    std::string severity = stringify_severity(s);
    std::string time = format_time("%F|%T");

    write_req_t* req = new write_req_t;

    std::string final_msg = "(" + time + "): " + severity + ": " + msg + "\n";
    char* msg_data = new char[final_msg.size()];
    std::memcpy(msg_data, final_msg.data(), final_msg.size());

    req->buf = uv_buf_init(msg_data, final_msg.size());

    uv_fs_write(this->loop_, &req->req, 1, &req->buf, 1, -1, after_write);
  }

  void Logger::step() noexcept
  {
    uv_run(this->loop_, UV_RUN_NOWAIT);
  }
}
