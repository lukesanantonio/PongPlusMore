/* * PpM - Pong Plus More - A pong clone full of surprises written with C++11.
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
#include "engine.h"

#include "common/vector.h"
namespace engine
{
  void add_core_methods(pong::Req_Dispatcher& dispatch, State& state,
                        pong::Logger& log)
  {
    using pong::Severity;
    namespace math = pong::math;
    using pong::Error_Response;

    using res_t = pong::response_result;

    dispatch.add_method<Severity, std::string>("Core.Log",
    [&log](Severity s, std::string msg) -> res_t
    {
      log.log(s, "(Plugin) " + msg);
      return Json::Value(true);
    });

    // Error codes: 1 - Engine already started.
    // Error codes: 2 - Window creation failure.
    dispatch.add_method<math::vector<int>, std::string>("Core.Start",
    [&state, &log](math::vector<int> extent, std::string const& title) -> res_t
    {
      if(state.window)
      {
        return Error_Response{1, "Engine already started"};
      }

      state.window = SDL_CreateWindow(title.c_str(),
                                      SDL_WINDOWPOS_UNDEFINED,
                                      SDL_WINDOWPOS_UNDEFINED,
                                      extent.x, extent.y, 0);

      state.renderer = SDL_CreateRenderer(state.window, -1,
                                          SDL_RENDERER_ACCELERATED);
      if(!state.window)
      {
        return Error_Response{2, "Failed to create window"};
      }

      return Json::Value(true);
    });
  }
}
