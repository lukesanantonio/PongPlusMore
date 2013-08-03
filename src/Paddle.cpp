/*!
 * \file Paddle.cpp
 * \brief Defines functions for the Paddle class.
 */
#include "Paddle.h"
namespace pong
{
  void Paddle::render(SDL_Surface* surface) const
  {
    SDL_Rect dest;

    dest.x = this->pos_.x;
    dest.y = this->pos_.y;

    SDL_FillRect(surface, &dest, SDL_MapRGB(surface->format, 0xff, 0xff, 0xff));
  }
};
