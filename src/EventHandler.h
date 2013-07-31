#pragma once
#include "SDL.h"
namespace pong
{
  struct EventHandler
  {
    virtual ~EventHandler() = default;

    virtual void handleEvent(const SDL_Event& event) = 0;
  };
};

