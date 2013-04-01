/*!
 * \file Label.cpp
 * \brief Contains definitions for the Label class.
 */
#include "Label.h"
#include "render_text.h"
namespace pong
{
  Label::Label(const std::string& text,
               std::size_t text_height,
               math::vector pos) noexcept :
               text_(text),
               text_height_(text_height),
               pos_(pos){}
  Label::~Label() noexcept
  {
    if(this->cached_surface_)
    {
      SDL_FreeSurface(this->cached_surface_);
    }
  }

  void Label::render(SDL_Surface* surface) const
  {
    this->generateSurface();
    SDL_Rect dest;
    dest.x = pos_.x;
    dest.y = pos_.y;

    SDL_BlitSurface(this->cached_surface_, NULL, surface, &dest);
  }

  void Label::generateSurface() const
  {
    if(this->cache_out_of_date_)
    {
      if(this->cached_surface_)
      {
        SDL_FreeSurface(this->cached_surface_);
      }
      this->cached_surface_ = render_text(this->text_, this->text_height_);
      this->cache_out_of_date_ = false;
    }
  }
};
