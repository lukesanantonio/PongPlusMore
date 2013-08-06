#pragma once
#include <memory>
#include <string>
#include "render_text.h"
namespace pong
{
  class GameSettings
  {
  public:
    std::unique_ptr<FontRenderer> font_renderer;
    unsigned int width;
    unsigned int height;
  };
  GameSettings loadSettings(const std::string& filename) noexcept;
};
