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
#include "GameSettings.h"
#include <fstream>
#include <json/json.h>
#include "common.h"
namespace pong
{
  GameSettings loadSettings(const std::string& filename) noexcept
  {
    GameSettings settings;

    try
    {

      std::ifstream input_file(filename);
      if(!input_file.good())
      {
        crash("Failed to open file: \"" + filename + "\"");
      }

      Json::Reader reader {Json::Features::strictMode()};
      Json::Value root;

      if(!reader.parse(input_file, root))
      {
        crash("JSON parse error:\n" + reader.getFormattedErrorMessages());
      }


      //Load all the values, then deal with them!
      Json::Value font_renderer_value = root["Font"]["Type"];
      Json::Value font_file_value = root["Font"]["Path"];
      Json::Value width_value = root["Bounds"]["Width"];
      Json::Value height_value = root["Bounds"]["Height"];

      if(font_renderer_value == Json::Value::null)
      {
        crash("No Font::Path specified in config file!");
      }
      if(font_file_value == Json::Value::null)
      {
        crash("No Font::Type specified in config file!");
      }
      if(width_value == Json::Value::null)
      {
        crash("No width specified in config file!");
      }
      if(height_value == Json::Value::null)
      {
        crash("No height specified in config file!");
      }

      if(font_renderer_value.asString() == "Mono")
      {
        settings.font_renderer =
                            std::unique_ptr<FontRenderer>(
                            new MonoTextRenderer(font_file_value.asString()));
      }
      else
      {
        crash("Invalid FontRenderer specified in config file!");
      }

      settings.width = width_value.asInt();
      settings.height = height_value.asInt();
    }
    catch(std::exception& error)
    {
      crash("jsoncpp exception thrown! Unknown Error!");
    }
    return settings;
  }
};
