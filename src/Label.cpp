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
               pos_(pos)
  {
    //Text color default: white
    this->text_color_.r = 0xff;
    this->text_color_.g = 0xff;
    this->text_color_.b = 0xff;

    //Background color default: black
    this->back_color_.r = 0x00;
    this->back_color_.g = 0x00;
    this->back_color_.b = 0x00;
  }
  Label::Label(const std::string& text,
               std::size_t text_height,
               math::vector pos,
               SDL_Color text_color,
               SDL_Color back_color) noexcept :
               text_(text),
               text_height_(text_height),
               pos_(pos),
               text_color_(text_color),
               back_color_(back_color) {}

  Label::Label(const Label& label) noexcept :
               text_(label.text_),
               text_height_(label.text_height_),
               pos_(label.pos_),
               text_color_(label.text_color_),
               back_color_(label.back_color_){}

  Label::Label(Label&& label) noexcept :
               text_(std::move(label.text_)),
               text_height_(label.text_height_),
               pos_(label.pos_),
               text_color_(label.text_color_),
               back_color_(label.back_color_),
               CachedSurface(std::move(label)){}

  Label& Label::operator=(const Label& label) noexcept
  {
    this->text(label.text_);
    this->text_height(label.text_height_);

    this->position(label.pos_);

    this->text_color(label.text_color_);
    this->back_color(label.back_color_);
  }

  Label& Label::operator=(Label&& label) noexcept
  {
    CachedSurface::operator=(std::move(label));

    this->text(std::move(label.text_));
    this->text_height(label.text_height_);

    this->position(label.pos_);

    this->text_color(label.text_color_);
    this->back_color(label.back_color_);
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
    SDL_Surface* cached_surface = render_text(this->text_, this->text_height_,
                                              this->text_color_,
                                              this->back_color_);
    return cached_surface;
  }
};
