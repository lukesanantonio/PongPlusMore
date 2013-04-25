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
               text_(label.text_),
               text_height_(label.text_height_),
               pos_(label.pos_),
               invert_(label.invert_) {}

  Label::Label(Label&& label) noexcept :
               text_(std::move(label.text_)),
               text_height_(label.text_height_),
               pos_(label.pos_),
               invert_(label.invert_),
               cache_out_of_date_(label.cache_out_of_date_)
  {
    //Point our cache to the cache of the object to be moved.
    this->cached_surface_ = label.cached_surface_;

    //Set the label to regenerate the cache if, for some strange reason, it
    //needs to as well as set the cached surface of the object to be moved from
    //to a nullptr to prevent it from being freed, which would screw everything
    //up.
    label.cache_out_of_date_ = true;
    label.cached_surface_ = nullptr;
  }

  Label& Label::operator=(const Label& label) noexcept
  {
    this->text(label.text_);
    this->text_height(label.text_height_);

    this->position(label.pos_);

    this->invert(label.invert_);

    //Just in case!
    this->cache_out_of_date_ = true;
  }

  Label& Label::operator=(Label&& label) noexcept
  {
    this->text(std::move(label.text_));
    this->text_height(label.text_height_);

    this->position(label.pos_);

    this->invert(label.invert_);

    //Now steal the objects cache:
    this->cache_out_of_date_ = label.cache_out_of_date_;

    this->cached_surface_ = label.cached_surface_;

    //Just in case!
    label.cache_out_of_date_ = true;
    label.cached_surface_ = nullptr;
  }

  void Label::render(SDL_Surface* surface) const
  {
    this->render(surface, this->pos_);
  }
  void Label::render(SDL_Surface* surface, math::vector pos) const
  {
    this->generateSurface();
    SDL_Rect dest;
    dest.x = pos.x;
    dest.y = pos.y;

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
