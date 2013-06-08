/*!
 * \file Paddle.cpp
 * \brief Defines functions for the Paddle class and the PaddleSignals class.
 */
#include "Paddle.h"
#include "render_text.h"
namespace pong
{
  void Paddle::render(SDL_Surface* surface) const
  {
  }
  SDL_Surface* Paddle::generateCache_private() const
  {
    SDL_Color color;
    color.r = 0xff;
    color.g = 0xff;
    color.b = 0xff;
    SDL_Surface* cache = generateRectangle(100, 10, color);
    return cache;
  }
};
