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
      Json::Value font_renderer_value = root["FontRenderer"];
      Json::Value width_value = root["Bounds"]["Width"];
      Json::Value height_value = root["Bounds"]["Height"];

      if(font_renderer_value == Json::Value::null)
      {
        crash("No FontRenderer specified in config file!");
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
                            std::unique_ptr<FontRenderer>(new MonoTextRenderer);
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
