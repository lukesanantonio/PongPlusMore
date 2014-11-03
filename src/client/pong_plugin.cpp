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
#include <iostream>
#include <vector>
#include <deque>
#include <chrono>
#include <thread>
#include <functional>

#include "core/plugin/req.h"
#include "core/plugin/response.h"
#include "core/common/vector.h"
#include "core/common/pif/helper.h"
#include "core/common/Timer.hpp"

namespace client
{
  struct Client
  {
    using cb_t = std::function<void(pong::Response const&)>;

    bool poll_request(pong::Request& req) noexcept;

    void post_request(pong::Request const& req) noexcept;
    void post_request(pong::Request const& req, cb_t callback) noexcept;

    void step() noexcept;
  private:
    std::deque<pong::Request> reqs_;
    std::vector<std::pair<pong::req_id_t, cb_t> > cbs_;
  };

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

  enum class Launch_State
  {
    Not_Running,
    Error,
    Running
  };

  struct State
  {
    Launch_State run_state = Launch_State::Not_Running;
  };

  template <class Type>
  void make_params_impl(Json::Value& val, Type const& t)
  {
    val.append(FORMATTER_TYPE(Type)::dump(t));
  }
  template <class Type, class... Types>
  void make_params_impl(Json::Value& val, Type const& t1, Types&&... params)
  {
    // Append the first one first,
    make_params_impl(val, t1);
    // Then the others.
    make_params_impl(val, std::forward<Types>(params)...);
  }

  template <class... Types>
  Json::Value make_params(Types&&... params)
  {
    Json::Value val(Json::ValueType::arrayValue);;
    make_params_impl(val, std::forward<Types>(params)...);
    return val;
  }
}

int main(int argc, char** argv)
{
  // Sleep a bit before starting.
  std::this_thread::sleep_for(std::chrono::seconds(1));

  client::Client c;
  client::State state;

  using namespace std::literals;
  using namespace pong::literals;

  using client::Launch_State;

  // Start the engine with 1000x1000 window.
  using pong::math::vector;
  using client::make_params;

  auto const& params = make_params(vector<int>(1000, 1000), "Pong Plus More"s);
  c.post_request({1_id, "Core.Start", params},
  [&state](pong::Response const& res)
  {
    // If we have an error on our hands...
    if(pong::is_error_response(res))
    {
      // TODO Log: err.msg + " (Error code: " + res.code + ")"
      state.run_state = Launch_State::Error;
    }
    // Successful start!
    else
    {
      state.run_state = Launch_State::Running;
    }
  });

  {
    pong::Timer<> run_timer;
    while(state.run_state == Launch_State::Not_Running)
    {
      if(run_timer.has_been(std::chrono::seconds(5))) break;
      c.step();
    }

    switch(state.run_state)
    {
      case Launch_State::Not_Running:
      {
        // No connection to engine. TODO log this message.
        return EXIT_FAILURE;
      }
      case Launch_State::Error:
      {
        // Failed to launch for some reason. TODO log this.
        return EXIT_FAILURE;
      }
      // If the engine started successfully let control continue.
      case Launch_State::Running: break;
    }
  }

  // We are running now.

  // Wait for 2 seconds before exiting.
  std::this_thread::sleep_for(std::chrono::seconds(2));

  // Exit/close the engine/window.
  c.post_request({boost::none_t{}, "Core.Exit"});
}
