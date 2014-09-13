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
#include "settings.h"
#include <fstream>
#include "common/deserialize.h"
namespace pong
{
  ClientSettings::ClientSettings() noexcept :
                             ClientSettings("/home/luke/.fonts/Railway.ttf") {}

  ClientSettings::ClientSettings(std::string const& filename) noexcept :
                            font(std::make_unique<MonoTextRenderer>(filename)),
                            extent(1000, 1000), plugins() {}

  ClientSettings load_config(Logger& l) noexcept
  {
    Json::Reader read(Json::Features::strictMode());
    Json::Value root;
    {
      std::ifstream input("resources/config.json");
      if(!read.parse(input, root))
      {
        l.log(Severity::Error, read.getFormattedErrorMessages());
        return ClientSettings{};
      }
    }

    ClientSettings settings(root["Font"]["Path"].asString());

    settings.extent.x = root["Extent"]["Width"].asInt();
    settings.extent.y = root["Extent"]["Height"].asInt();

    for(Json::Value plugin : root["Plugins"])
    {
      PluginInfo info;
      info.file = plugin["file"].asString();
      info.enabled = plugin["enabled"].asBool();
      settings.plugins.push_back(info);
    }

    settings.render_debug = root["RenderDebug"].asBool();

    return settings;
  }
}
