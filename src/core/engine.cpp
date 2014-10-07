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
#include <memory>
#include <fstream>
#include <uv.h>
#include "SDL.h"
#include "json/json.h"

#include "core/plugin/plugins.h"
#include "core/io/Logger.h"

std::unique_ptr<pong::Plugin> spawn_plugin(Json::Value const& json) noexcept
{
  std::vector<char*> args;

  for(Json::Value const& array_element : json["plugin"])
  {
    args.push_back((char*) array_element.asCString());
  }
  args.push_back(NULL);

  pong::ipc::Spawn_Options opts;
  opts.args = &args[0];
  opts.cwd = NULL;
  return pong::make_json_plugin<pong::Child_Process>(opts);
}

struct State
{
  bool running = true;
  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;
};

struct Request_Visitor : public boost::static_visitor<pong::Response>
{
  Request_Visitor(State& state, pong::Logger& l) noexcept
                  : state_(state), log_(l) {}
  template <class Req_Type>
  pong::Response operator()(Req_Type const& req) const noexcept
  {
    return pong::Response{req.id, "Unimplemented request", false};
  }

  pong::Response operator()(pong::Log_Req const& req) const noexcept
  {
    log_.log(std::get<0>(req.params),
             "(Engine Mod:) " + std::get<1>(req.params));
    return pong::Response{req.id, true, false};
  }
  pong::Response operator()(pong::Exit_Req const& req) const noexcept
  {
    if(state_.running == false)
    {
      // We are already in the process of exiting!
      return pong::Response{req.id, false, false};
    }
    log_.log(pong::Severity::Info, "Exit request recieved");
    state_.running = false;
    // Success, we will exit soon!
    return pong::Response{req.id, true, false};
  }
  pong::Response operator()(pong::New_Window_Req const& req) const noexcept
  {
    log_.log(pong::Severity::Info, "Creating window");

    pong::math::vector<int> const& dimensions = std::get<1>(req.params);

    state_.window = SDL_CreateWindow(std::get<0>(req.params).c_str(),
                                     SDL_WINDOWPOS_UNDEFINED,
                                     SDL_WINDOWPOS_UNDEFINED,
                                     dimensions.x, dimensions.y, 0);
    if(!state_.window)
    {
      log_.log(pong::Severity::Error, "Failed to create window (" +
               std::to_string(dimensions.x) + "x" +
               std::to_string(dimensions.y) + ")");

      // Return a false boolean, indicating failure.
      return pong::Response{req.id, false, false};
    }

    state_.renderer = SDL_CreateRenderer(state_.window, -1,
                                         SDL_RENDERER_ACCELERATED);
    if(!state_.renderer)
    {
      log_.log(pong::Severity::Error, "Failed to create window renderer");

      // Return a false boolean, indicating failure.
      return pong::Response{req.id, false, false};
    }

    // Success!
    return pong::Response{req.id, true, false};
  }
private:
  State& state_;
  pong::Logger& log_;
};

struct Request_Id_Visitor : public boost::static_visitor<pong::req_id>
{
  template <class Req>
  pong::req_id operator()(Req const& req) const noexcept
  {
    return req.id;
  }
};

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
  std::unique_ptr<pong::Plugin> plugin = spawn_plugin(json);

  State state;
  Request_Visitor req_visit(state, log);

  while(state.running)
  {
    pong::Request req;
    while(plugin->poll_request(req))
    {
      pong::Response res = boost::apply_visitor(req_visit, req);
      if(boost::apply_visitor(Request_Id_Visitor(), req))
      {
        plugin->post_response(res);
      }
    }
  }

  return 0;
}
