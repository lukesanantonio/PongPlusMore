/*
 * PpM - Pong Plus More - A pong clone full of surprises written with C++11.
 * Copyright (C) 2013  Luke San Antonio
 *
 * You can contact me (Luke San Antonio) at lukesanantonio@gmail.com!
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/*!
 * \file Label.cpp
 * \brief Contains definitions for the Label class.
 */
#include <boost/lexical_cast.hpp>
#include "common/crash.hpp"
#include <vector>
namespace pong
{
  /*!
   * \brief Returns the width of the cached text surface.
   *
   * \returns cache_.cache()->w
   *
   * \post Generates the cache if necessary.
   * \warning This function requires a valid FontRenderer, otherwise it
   * crashes.
   */
  template <class Data>
  inline int Label<Data>::surface_width() const
  {
    return this->cache_.template grab_dependency<0>().cache()->w;
  }
  /*!
   * \brief Returns the height of the cached text surface.
   *
   * \returns cache_.cache()->h
   *
   * \post Generates the cache if necessary.
   * \warning This function requires a valid FontRenderer, otherwise it
   * crashes.
   */
  template <class Data>
  inline int Label<Data>::surface_height() const
  {
    return this->cache_.template grab_dependency<0>().cache()->h;
  }

  /*!
   * \brief Sets the content of the label.
   *
   * \post Invalidates the cache if the passed in content is different from the
   * one already stored.
   */
  template <class Data>
  inline void Label<Data>::data(const Data& data) noexcept
  {
    if(this->data_ == data) return;
    this->data_ = data;
    this->cache_.template grab_dependency<0>().invalidate();
    this->cache_.invalidate();
  }
  /*!
   * \brief Gets the content of the label.
   *
   * \returns Label::data_
   */
  template <class Data>
  inline Data Label<Data>::data() const noexcept
  {
    return this->data_;
  }

  /*!
   * \brief Sets the text height of the label.
   *
   * \post Invalidates the cache if the passed in text height is different
   * from the one already stored.
   */
  template <class Data>
  inline void Label<Data>::text_height(int text_height) noexcept
  {
    if(this->text_height_ == text_height) return;
    this->text_height_ = text_height;
    this->cache_.template grab_dependency<0>().invalidate();
    this->cache_.invalidate();
  }
  /*!
   * \brief Returns the text height of the label.
   *
   * \returns Label::text_height_
   */
  template <class Data>
  inline int Label<Data>::text_height() const noexcept
  {
    return this->text_height_;
  }

  /*!
   * \brief Sets the position of the top left corner of the surface.
   *
   * Does not invalidate the surface.
   */
  template <class Data>
  inline void Label<Data>::position(math::vector<int> pos) noexcept
  {
    this->pos_ = pos;
  }
  /*!
   * \brief Returns the position of the top left corner of the surface.
   *
   * \returns Label::pos_
   */
  template <class Data>
  inline math::vector<int> Label<Data>::position() const noexcept
  {
    return this->pos_;
  }

  /*!
   * \brief Sets the color of the text.
   *
   * \post Invalidates the cache if the passed in text color is different
   * from the one already stored.
   */
  template <class Data>
  inline void Label<Data>::text_color(SDL_Color text_color) noexcept
  {
    if(this->text_color_ == text_color) return;
    this->text_color_ = text_color;
    this->cache_.template grab_dependency<0>().invalidate();
    this->cache_.invalidate();
  }
  /*!
   * \brief Returns the color of the text.
   *
   * \returns Label::text_color_
   */
  template <class Data>
  inline SDL_Color Label<Data>::text_color() const noexcept
  {
    return this->text_color_;
  }

  /*!
   * \brief Sets the font renderer implementation to use.
   *
   * \post Invalidates the cache if the passed in font renderer is different
   * from the one already stored.
   */
  template <class Data>
  inline void Label<Data>::font_renderer(FontRenderer* font_renderer) noexcept
  {
    if(this->font_renderer_ == font_renderer) return;
    this->font_renderer_ = font_renderer;
    this->cache_.template grab_dependency<0>().invalidate();
    this->cache_.invalidate();
  }
  /*!
   * \brief Returns the font renderer.
   */
  template <class Data>
  inline FontRenderer* Label<Data>::font_renderer() const noexcept
  {
    return this->font_renderer_;
  }

  template <class Data>
  Texture_Cache make_label_cache(Label<Data>& l)
  {
    Texture_Cache c;
    c.gen_func([](Texture_Cache::ptr_type p,
                  Surface_Cache& s, SDL_Renderer*& r)
    {
      if(p) return p;
      p.reset(SDL_CreateTextureFromSurface(r, s.cache()));
      return p;
    });

    c.grab_dependency<0>().gen_func(
    [&l](Surface_Cache::ptr_type p)
    {
      if(p) return p;
      std::string text = boost::lexical_cast<std::string>(l.data());

      // Lines?
      std::vector<std::string> lines;

      using std::begin; using std::end;
      std::string::iterator prev_found = begin(text);
      std::string::iterator found;
      while((found = std::find(prev_found, end(text), '\n')) != end(text))
      {
        lines.push_back(std::string(prev_found, found));
        prev_found = found + 1;
      }
      if(prev_found != end(text))
      {
        lines.push_back(std::string(prev_found, end(text)));
      }

      if(lines.empty()) return Surface_Cache::ptr_type(nullptr);

      SDL_Color back_color = l.text_color();
      back_color.a = 0x00;

      std::vector<UniquePtrSurface> line_surfs;
      for(std::string line : lines)
      {
        line_surfs.push_back(l.font_renderer()->render_text(line,
                                                            l.text_height(),
                                                            l.text_color(),
                                                            back_color));
      }

      int width = 0;
      int line_height = l.text_height() * 1.2;
      int line_pen = line_height * (lines.size() - 1);
      for(UniquePtrSurface const& surf : line_surfs)
      {
        width = std::max(width, surf->w);
      }
      int height = lines.size() == 1 ? line_surfs.front()->h : l.text_height();

      SDL_Surface* result = SDL_CreateRGBSurface(0, width, height + line_pen,
                                               8, 0, 0, 0, 0);
      SDL_SetSurfaceBlendMode(result, SDL_BLENDMODE_BLEND);
      initialize_grayscale_palette(result, l.text_color(), back_color);

      int pen_y = 0;
      for(UniquePtrSurface const& surf : line_surfs)
      {
        SDL_Rect dest;
        dest.x = 0;
        dest.y = pen_y;
        SDL_SetSurfaceBlendMode(surf.get(), SDL_BLENDMODE_NONE);
        SDL_BlitSurface(surf.get(), NULL, result, &dest);
        pen_y += line_height;
      }

      return Surface_Cache::ptr_type(result);
    });
    return c;
  }

  /*!
   * \brief Initializes a label with everything necessary to start rendering.
   *
   * \param data The content to display. Converted to a string and passed to
   * pong::render_text.
   * \param text_height The height of the text in pixels. Passed to
   * pong::render_text.
   * \param pos The position of the top left corner where the surface will be
   * blitted.
   * \param text_color The color of the text.
   * \param font_renderer The font renderer implementation to use when
   * rasterizing the text!
   *
   * \note The text will be rendered monochrome in white. The background will
   * be solid black.
   */
  template <class Data>
  Label<Data>::Label(const Data& data,
                     int text_height,
                     math::vector<int> pos,
                     SDL_Color text_color,
                     FontRenderer* font_renderer) noexcept :
                     data_(data),
                     text_height_(text_height),
                     pos_(pos),
                     text_color_(text_color),
                     font_renderer_(font_renderer),
                     cache_(make_label_cache(*this)) {}

  /*!
   * \brief Copy constructor.
   *
   * Does not copy the cache.
   */
  template <class Data>
  Label<Data>::Label(const Label& label) noexcept :
                     data_(label.data_),
                     text_height_(label.text_height_),
                     pos_(label.pos_),
                     text_color_(label.text_color_),
                     font_renderer_(label.font_renderer_),
                     cache_(label.cache_){}
  /*!
   * \brief Move constructor.
   *
   * Moves the cache from the object passed in, notably.
   */
  template <class Data>
  Label<Data>::Label(Label&& label) noexcept :
                     data_(std::move(label.data_)),
                     text_height_(label.text_height_),
                     pos_(label.pos_),
                     text_color_(label.text_color_),
                     font_renderer_(label.font_renderer_),
                     cache_(std::move(label.cache_)) {}

  /*!
   * \brief Copy assignment operator.
   *
   * Does not copy the cache, but invalidates it, just in case.
   */
  template <class Data>
  Label<Data>& Label<Data>::operator=(const Label& label) noexcept
  {
    this->data(label.data_);
    this->text_height(label.text_height_);

    this->position(label.pos_);

    this->text_color(label.text_color_);
    this->font_renderer(label.font_renderer_);

    this->cache_ = label.cache_;

    return *this;
  }

  /*!
   * \brief Move assignment operator.
   *
   * Moves the cache from the other object to `this` one, notably.
   */
  template <class Data>
  Label<Data>& Label<Data>::operator=(Label&& label) noexcept
  {
    this->data(std::move(label.data_));
    this->text_height(label.text_height_);

    this->position(label.pos_);

    this->text_color(label.text_color_);
    this->font_renderer(label.font_renderer_);

    this->cache_ = std::move(label.cache_);

    return *this;
  }

  /*!
   * \brief Renders the label using the passed in renderer.
   */
  template <class Data>
  void Label<Data>::render(SDL_Renderer* renderer) const
  {
    if(!renderer) return;

    this->cache_.template set_dependency<1>(renderer);

    if(!this->cache_.cache()) return;

    SDL_Rect dest;
    dest.x = this->pos_.x;
    dest.y = this->pos_.y;
    dest.w = this->surface_width();
    dest.h = this->surface_height();
    SDL_RenderCopy(renderer, this->cache_.cache(), NULL, &dest);
  }
  template <class Data>
  void Label<Data>::render(SDL_Surface* surface) const
  {
    SDL_Surface* src = this->cache_.template grab_dependency<0>().cache();
    if(!src) return;

    SDL_Rect dest;
    dest.x = this->pos_.x;
    dest.y = this->pos_.y;
    dest.w = this->surface_width();
    dest.h = this->surface_height();
    SDL_BlitSurface(src, NULL, surface, &dest);
  }
}