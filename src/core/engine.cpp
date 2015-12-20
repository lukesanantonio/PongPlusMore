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

#include <iostream>
#include <memory>
#include <fstream>
#include <uv.h>

#include <msgpack.hpp>

#include "SDL.h"
#include "json/json.h"

#include "core/plugin/plugins.h"
#include "core/common/log.h"

#include "core/plugin/Req_Dispatcher.h"

namespace engine
{
  void step(State& state) noexcept
  {
    if(state.freeze) return;

    {
      auto col = state.clear_color;
      SDL_SetRenderDrawColor(state.renderer, col.r, col.g, col.b, col.a);
    }

    SDL_RenderClear(state.renderer);

    for(auto const& label_pair : state.labels_)
    {
      pong::Label<std::string> const& label = std::get<1>(label_pair);
      label.render(state.renderer);
    }

    SDL_RenderPresent(state.renderer);
  }
}

int main(int argc, char** argv)
{
  if(argc < 2)
  {
    std::cerr << "usage: " << argv[0] << " <plugin>" << std::endl;
    return EXIT_FAILURE;
  }

  pong::Scoped_Log_Init log_init{};

  PONG_LOG_I("Spawning mod: %", argv[1]);
  //pong::Json_Plugin plugin = engine::spawn_plugin(json);

  //engine::State state;
  //pong::Req_Dispatcher dispatch;

  // Populate the dispatcher!
  //engine::add_core_methods(dispatch, state, log);

  //while(state.running)
  {
    //pong::Request req;
    //while(plugin.poll_request(req))
    {
      //pong::Response res = dispatch.dispatch(req);

      // If the id is valid it means the requester wants a response back.
      //if(res.id)
      {
        // The id we know is going to be true, so we can forget about the
        // exception that is thrown when it isn't.
        //plugin.post_response(res);
      }
    }

    //engine::step(state);
  }

  return 0;
}
