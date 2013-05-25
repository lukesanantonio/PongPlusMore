/*!
 * \file EventSignals.cpp
 * \brief File basically containing the main event-checking loop. This is where
 * it happens.
 */
#include "EventSignals.h"
namespace pong
{
  void handleEvents(EventSignals& signals)
  {
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
      switch(event.type)
      {
        case SDL_KEYDOWN:
        case SDL_QUIT:
        {
          signals.on_quit();
          break;
        }
        case SDL_MOUSEBUTTONUP:
        {
          math::vector pos;
          pos.x = event.button.x;
          pos.y = event.button.y;
          signals.on_mouse_click(pos);
          break;
        }
        case SDL_MOUSEMOTION:
        {
          math::vector pos;
          pos.x = event.motion.x;
          pos.y = event.motion.y;
          signals.on_mouse_motion(pos);
          break;
        }
      }
    }
  }
};
