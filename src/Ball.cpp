/*!
 * \file Ball.cpp
 * \brief File containing the definitions of the Ball class.
 */
#include "Ball.h"
namespace pong
{
  void Ball::render(SDL_Surface* surface) const
  {
    SDL_Rect dest;
    dest.x = this->pos_.x;
    dest.y = this->pos_.y;
    dest.w = this->diameter_;
    dest.h = this->diameter_;


    SDL_FillRect(surface, &dest, SDL_MapRGB(surface->format, 0xff, 0xff, 0xff));
  }
};
