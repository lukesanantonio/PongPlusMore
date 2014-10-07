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
#include <cstring>
namespace pong
{
  Logger::Logger(Logger_Stream s) noexcept
                 : min_severity_(static_cast<Severity>(0))
  {
    this->loop_ = uv_loop_new();

    if(s == Logger_Stream::Out) fd_ = 1;
    else if(s == Logger_Stream::Err) fd_ = 2;
    else fd_ = -1;

    should_close_ = false;
  }
  Logger::Logger(std::string const& fname) noexcept : Logger()
  {
    uv_fs_t req;
    int err = uv_fs_open(loop_, &req, fname.c_str(),
                         O_APPEND | O_CREAT, 0, NULL);
    if(err == 0)
    {
      fd_ = req.result;
      should_close_ = true;
    }
    else
    {
      fd_ = -1;
      should_close_ = false;
    }
  }
  Logger::~Logger() noexcept
  {
    uv_run(this->loop_, UV_RUN_DEFAULT);

    if(should_close_)
    {
      uv_fs_t req;
      uv_fs_close(loop_, &req, fd_, NULL);
    }

    uv_loop_delete(this->loop_);
  }

  std::string stringify_severity(const Severity s)
  {
    std::string severity;
    if(s == Severity::Debug) severity = "Debug";
    else if(s == Severity::Info) severity = "Info";
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
    // Don't bother if we are a null logger by chance.
    if(fd_ == -1) return;

    // Also don't bother if our msg doesn't qualify to be sent out.
    if(static_cast<int>(min_severity_) > static_cast<int>(s)) return;

    // Stringify severity
    std::string severity = stringify_severity(s);
    std::string time = format_time("%F|%T");

    write_req_t* req = new write_req_t;

    std::string final_msg = "(" + time + "): " + severity + ": " + msg + "\n";
    char* msg_data = new char[final_msg.size()];
    std::memcpy(msg_data, final_msg.data(), final_msg.size());

    req->buf = uv_buf_init(msg_data, final_msg.size());

    uv_fs_write(this->loop_, &req->req, fd_, &req->buf, 1, -1, after_write);
  }

  void Logger::step() noexcept
  {
    uv_run(this->loop_, UV_RUN_NOWAIT);
  }
}
BEGIN_FORMATTER_SCOPE
{
  DEFINE_FORMATTABLE_ENUM(pong::Severity);
}
END_FORMATTER_SCOPE
