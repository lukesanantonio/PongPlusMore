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
    SDL_Rect dest;

    dest.x = this->pos_.x;
    dest.y = this->pos_.y;

    SDL_BlitSurface(this->cache(), nullptr, surface, &dest);
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
  void Paddle::update()
  {
    //If the paddle controller is a nullptr, exit immediately, This function is
    //a no-op.
    if(!this->controller_) return;
    this->controller_->update(this);
  }
};
