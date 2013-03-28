#include "Label.h"
#include "render_text.h"
namespace pong
{
  Label::Label(const std::string& text,
               std::size_t text_size,
               math::vector pos) : text_(text),
                                   text_size_(text_size),
                                   pos_(pos){}
  
  Label::~Label() noexcept
  {
    //Free the cached text surface if necessary.
    if(this->cached_surface_)
    {
      SDL_FreeSurface(this->cached_surface_);
    }
  }
  
  void Label::render(SDL_Surface* surface) const
  {
    //Make sure the client actually wants to render.
    if(this->text_ != "" && this->text_size_)
    {
      //Do we need to render again.
      if(this->cache_out_of_date_)
      {
        this->cacheSurface();
      }
      SDL_Rect dest;
      dest.x = this->pos_.x;
      dest.y = this->pos_.y;
      SDL_BlitSurface(this->cached_surface_, NULL, surface, &dest);
    }
  }
  void Label::cacheSurface() const
  {
    //Do we need to free the surface first?
    if(this->cached_surface_)
    {
      SDL_FreeSurface(this->cached_surface_);
    }
    this->cached_surface_ = render_text(this->text_, this->text_size_);
    this->cache_out_of_date_ = false;
  }
  
  SDL_Surface* Label::getCachedSurface() const noexcept
  {
    return this->cached_surface_;
  }
  void Label::setText(const std::string& text) noexcept
  {
    this->text_ = text;
    this->cache_out_of_date_ = true;
  }
  std::string Label::getText() const noexcept
  {
    return this->text_;
  }
  void Label::setTextSize(std::size_t text_size) noexcept
  {
    this->text_size_ = text_size;
    this->cache_out_of_date_ = true;
  }
  std::size_t Label::getTextSize() const noexcept
  {
    return this->text_size_;
  }
  void Label::setPosition(math::vector pos) noexcept
  {
    this->pos_ = pos;
  }
  math::vector Label::getPosition() const noexcept
  {
    return this->pos_;
  }
};
