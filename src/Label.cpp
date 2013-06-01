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

  Label::Label(const Label& label) noexcept :
               text_(label.text_),
               text_height_(label.text_height_),
               pos_(label.pos_),
               invert_(label.invert_) {}

  Label::Label(Label&& label) noexcept :
               text_(std::move(label.text_)),
               text_height_(label.text_height_),
               pos_(label.pos_),
               invert_(label.invert_),
               CachedSurface(std::move(label)){}

  Label& Label::operator=(const Label& label) noexcept
  {
    this->text(label.text_);
    this->text_height(label.text_height_);

    this->position(label.pos_);

    this->invert(label.invert_);

    //Just to make sure, regenerate the cache.
    this->invalidateCache();
  }

  Label& Label::operator=(Label&& label) noexcept
  {
    CachedSurface::operator=(std::move(label));

    this->text(std::move(label.text_));
    this->text_height(label.text_height_);

    this->position(label.pos_);

    this->invert(label.invert_);
  }

  void Label::render(SDL_Surface* surface) const
  {
    this->render(surface, this->pos_);
  }
  void Label::render(SDL_Surface* surface, math::vector pos) const
  {
    SDL_Rect dest;
    dest.x = pos.x;
    dest.y = pos.y;

    SDL_BlitSurface(this->cache(), NULL, surface, &dest);
  }

  SDL_Surface* Label::generateCache_private() const
  {
    SDL_Surface* cached_surface = render_text(this->text_, this->text_height_);
    if(this->invert_)
    {
      invertPalette(cached_surface);
    }
    return cached_surface;
  }
};
