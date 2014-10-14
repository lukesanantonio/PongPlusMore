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
#include "../common/utility.h"
#include "../common/pif/helper.h"
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

  bool Json_Plugin::poll_request(Request& req)
  {
    io_->step();
    if(!bufs_.size()) return false;

    Json::Reader read(Json::Features::strictMode());

    Json::Value val;
    using std::begin; using std::end;

    std::string doc(begin(bufs_.front()), end(bufs_.front()));
    bufs_.pop();

    if(read.parse(doc, val))
    {
      req = FORMATTER_TYPE(Request)::parse(val);
    }
    else
    {
      throw Json_Parse_Error{doc, read.getFormattedErrorMessages()};
    }
    return true;
  }

  void Json_Plugin::post_response(Response const& res) noexcept
  {
    Json::FastWriter w;
    std::string s = w.write(FORMATTER_TYPE(Response)::dump(res));
    io_->write(vec_from_string(s));
    io_->step();
  }
}
