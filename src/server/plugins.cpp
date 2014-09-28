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
#include "plugins.h"
namespace pong
{
  Json_Plugin::Json_Plugin(std::unique_ptr<External_IO> io) noexcept
                           : io_(std::move(io))
  {
    io_->set_read_callback([this](const std::vector<char>& buf)
    {
      bufs_.push(buf);
    });
  }
  bool Json_Plugin::poll_request(net::req::Request& req) noexcept
  {
    io_->step();
    if(!bufs_.size()) return false;

    Json::Value val;

    Json::Reader read(Json::Features::strictMode());

    using std::begin; using std::end;
    // While the queued buffer is failing to compile.
    while(!read.parse(std::string(begin(bufs_.front()), end(bufs_.front())),
                      val))
    {
      // Ignore it and continue on.
      bufs_.pop();
      if(!bufs_.size()) return false;
    }

    bufs_.pop();
    req = parse_request(val);
    return true;
  }

  void Json_Plugin::post_result(net::req::Request const& req) noexcept
  {
    Json::FastWriter w;
    net::req::Request_Base const& base_req = net::req::to_base(req);
    io_->write(vec_from_string(w.write(base_req.response_json())));
  }
}
