/*
 * uGlue - Glue many languages together into a whole with ukernel-inspired RPC.
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
#include "render/color.h"

#include "common/log.h"
namespace engine
{
  void add_core_methods(ug::Req_Dispatcher& dispatch, State& state)
  {
    using ug::Log_Severity;
    using ug::Error_Response;

    using ug::Vec;

    using res_t = ug::response_result;

    dispatch.add_method<Log_Severity, std::string>("Core.Log",
    [](Log_Severity s, std::string msg) -> res_t
    {
      log(s, "(Plugin) " + msg);
      return Json::Value(true);
    });

    // Error codes: 1 - Engine already started.
    // Error codes: 2 - Window creation failure.
    dispatch.add_method<Vec<int>, std::string>("Core.Start",
    [&state](Vec<int> extent, std::string const& title) -> res_t
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
      if(!state.window || !state.renderer)
      {
        return Error_Response{2, "Failed to create window"};
      }

      return Json::Value(true);
    });

    // Error code: 3 - engine now started.
    dispatch.add_method<>("Core.Exit",
    [&state]() -> res_t
    {
      if(!state.window || !state.renderer)
      {
        return Error_Response{3, "Engine not started"};
      }

      SDL_DestroyRenderer(state.renderer);
      SDL_DestroyWindow(state.window);
      state.running = false;

      return Json::Value(true);
    });

    dispatch.add_method<ug::Color>("Core.Set_Clear_Color",
    [&state](ug::Color const& c) -> res_t
    {
      state.clear_color = c;
      return Json::Value(true);
    });

    dispatch.add_method<bool>("Core.Set_Freeze",
    [&state](bool freeze) -> res_t
    {
      state.freeze = freeze;
      return Json::Value(true);
    });

    add_widget_methods(dispatch, state);
  }
}
