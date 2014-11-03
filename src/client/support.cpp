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
#include "support.h"

#include <iostream>
#include <algorithm>
#include "core/common/pif/helper.h"

namespace client
{
  bool Client::poll_request(pong::Request& req) noexcept
  {
    // Do we have any requests waiting?
    if(reqs_.size())
    {
      req = reqs_.front();
      reqs_.pop_front();
      return true;
    }
    return false;
  }
  void Client::post_request(pong::Request const& req) noexcept
  {
    // Dump the json to stdout.
    Json::Value value = FORMATTER_TYPE(pong::Request)::dump(req);
    Json::FastWriter fw;
    std::cout << fw.write(value) << std::flush;
  }
  void Client::post_request(pong::Request const& req, cb_t callback) noexcept
  {
    post_request(req);

    // Only track the response if we have a valid id.
    if(req.id)
    {
      cbs_.push_back({req.id.value(), callback});
    }
  }

  void Client::step() noexcept
  {
    std::string input;
    std::getline(std::cin, input);

    // Make sure the input is valid json.
    Json::Reader read{Json::Features::strictMode()};
    Json::Value val;
    if(!read.parse(input, val)) return;

    // We have valid json. Now try to parse it as a response, then as a
    // request if that fails.
    try
    {
      pong::Response res = FORMATTER_TYPE(pong::Response)::parse(val);

      // Could we be tracking this id?
      if(res.id)
      {
        using std::begin; using std::end;
        auto loc = std::find_if(begin(cbs_), end(cbs_),
        [&res](auto const& pair)
        {
          return res.id.value() == std::get<0>(pair);
        });
        if(loc != end(cbs_))
        {
          std::get<1>(*loc)(res);
          cbs_.erase(loc);
        }
      }

      // We found and successfully parsed a response. Our job is done for now.
      return;
    }
    catch(pong::Invalid_Response_Exception& e) {}

    try
    {
      // Valid request?
      pong::Request req = FORMATTER_TYPE(pong::Request)::parse(val);
      // Queue it up!
      reqs_.push_back(req);
    }
    catch(pong::Invalid_Request_Exception& e) {}
  }

  void log_message(Client& c, pong::Severity s, std::string const& m) noexcept
  {
    c.post_request({boost::none_t{}, "Core.Log", make_params(s, m)});
  }
}
