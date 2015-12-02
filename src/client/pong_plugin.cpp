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
#include <chrono>
#include <thread>
#include <functional>

#include "support.h"

#include "core/common/pif/helper.h"
#include "core/common/vector.h"
#include "core/common/Timer.hpp"

namespace client
{
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
  using client::make_params;

  auto const& params = make_params(pong::Vec<int>(1000, 1000), "Pong Plus More"s);
  c.post_request({1_id, "Core.Start", params},
  [&state, &c](pong::Response const& res)
  {
    // If we have an error on our hands...
    if(pong::is_error_response(res))
    {
      pong::Error_Response err = pong::get_error_response(res);
      client::log_message(c, pong::Severity::Error,
                          err.msg + " (Error code: " +
                          std::to_string(err.code) + ")");
      state.run_state = Launch_State::Error;
    }
    else
    {
      // Successful start!
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
        client::log_message(c, pong::Severity::Error,
                            "Timed out waiting for starting confirmation.");
        return EXIT_FAILURE;
      }
      case Launch_State::Error:
      {
        // Failed to launch for some reason. Already being logged in the
        // request callback.
        return EXIT_FAILURE;
      }
      // If the engine started successfully let control continue.
      case Launch_State::Running:
      {
        // Log the success!
        client::log_message(c, pong::Severity::Error,
                            "Successful engine start");
      }
    }
  }

  // We are running now.

  // Wait for 2 seconds before exiting.
  std::this_thread::sleep_for(std::chrono::seconds(2));

  // Exit/close the engine/window.
  c.post_request({boost::none_t{}, "Core.Exit"});
}
