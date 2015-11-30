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
#include "engine.h"

#include <iostream>
#include <memory>
#include <fstream>
#include <uv.h>
#include "SDL.h"
#include "json/json.h"

#include "core/plugin/plugins.h"
#include "core/io/Logger.h"

#include "core/plugin/Req_Dispatcher.h"

namespace engine
{
  pong::Json_Plugin spawn_plugin(Json::Value const& json)
  {
    std::vector<char*> args;

    for(Json::Value const& array_element : json["plugin"])
    {
      args.push_back((char*) array_element.asCString());
    }
    args.push_back(NULL);

    pong::ipc::Spawn_Options opts;
    opts.args = &args[0];
    opts.cwd = ".";
    try
    {
      auto json_plugin = pong::make_json_plugin<pong::Child_Process>(opts);
      return json_plugin;
    }
    catch(pong::ipc::Spawn_Error& e)
    {
      std::cerr << uv_strerror(e.err) << std::endl;
      throw;
    }
  }

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
    std::cerr << "usage: " << argv[0] << " <game-decl>" << std::endl;
    return EXIT_FAILURE;
  }

  Json::Reader read(Json::Features::strictMode());
  std::ifstream file(argv[1]);

  Json::Value json;
  read.parse(file, json);

  using pong::Logger; using pong::Logger_Stream; using pong::Severity;
  Logger log(Logger_Stream::Err);

  std::string cwd = json.get("cwd", "").asString();
  if(cwd.size())
  {
    log.log(Severity::Info, "Changing directory: '" + cwd + "'");
    uv_chdir(json.get("cwd", "").asString().c_str());
  }

  // Spawn the main plugin.
  if(!json.isMember("plugin"))
  {
    log.log(Severity::Error, "No mod specified in game decl spec");
    return EXIT_FAILURE;
  }

  log.log(Severity::Info, "Spawning mod: " + json["plugin"][0].asString());
  pong::Json_Plugin plugin = engine::spawn_plugin(json);

  engine::State state;
  pong::Req_Dispatcher dispatch;

  // Populate the dispatcher!
  engine::add_core_methods(dispatch, state, log);

  while(state.running)
  {
    pong::Request req;
    while(plugin.poll_request(req))
    {
      pong::Response res = dispatch.dispatch(req);

      // If the id is valid it means the requester wants a response back.
      if(res.id)
      {
        // The id we know is going to be true, so we can forget about the
        // exception that is thrown when it isn't.
        plugin.post_response(res);
      }
    }

    engine::step(state);
  }

  return 0;
}
