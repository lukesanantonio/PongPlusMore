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
               math::vector pos,
               bool invert) noexcept :
               text_(text),
               text_height_(text_height),
               pos_(pos),
               invert_(invert) {}
  Label::~Label() noexcept
  {
    if(this->cached_surface_)
    {
      SDL_FreeSurface(this->cached_surface_);
    }
  }

  Label::Label(const Label& label) noexcept :
               text_(label.text()),
               text_height_(label.text_height()),
               pos_(label.position()),
               invert_(label.invert()) {}

  Label& Label::operator=(const Label& label) noexcept
  {
    this->text(label.text());
    this->text_height(label.text_height());

    this->position(label.position());

    this->invert(label.invert());
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
      if(this->invert_)
      {
        invertPalette(this->cached_surface_);
      }
      this->cache_out_of_date_ = false;
    }
  }
};
