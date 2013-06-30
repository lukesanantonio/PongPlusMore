/*!
 * \file Ball.cpp
 * \brief File containing the static ball class definitions.
 */
#include "Ball.h"
#include "render_text.h"
namespace pong
{
  void Ball::render(SDL_Surface* surface) const
  {
    SDL_Rect dest;
    dest.x = this->pos_.x;
    dest.y = this->pos_.y;
    SDL_BlitSurface(this->cache(), NULL, surface, NULL);
  }
  SDL_Surface* Ball::generateCache_private() const
  {
    SDL_Color white;
    white.r = 0xff;
    white.g = 0xff;
    white.b = 0xff;
    return generateRectangle(this->diameter_, this->diameter_, white);
  }
};