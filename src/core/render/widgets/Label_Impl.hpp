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
#include <utility>
namespace pong
{
  /*!
   * \brief Returns the width of the cached text surface.
   *
   * \returns cache_.cache()->w
   *
   * \post Generates the cache if necessary.
   * \warning This function requires a valid Rasterizer, otherwise it
   * crashes.
   */
  template <class Data>
  inline int Label<Data>::surface_width() const
  {
    auto* dep = this->cache_.template grab_dependency<0>().cache();
    if(dep) return dep->surface->w;
    return 0;
  }
  /*!
   * \brief Returns the height of the cached text surface.
   *
   * \returns cache_.cache()->h
   *
   * \post Generates the cache if necessary.
   * \warning This function requires a valid Rasterizer, otherwise it
   * crashes.
   */
  template <class Data>
  inline int Label<Data>::surface_height() const
  {
    auto* dep = this->cache_.template grab_dependency<0>().cache();
    if(dep) return dep->surface->h;
    return 0;
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

  template <class Data>
  inline void Label<Data>::font_face(text::Face* face) noexcept
  {
    if(this->font_face_ == face) return;
    this->font_face_ = face;
    this->cache_.template grab_dependency<0>().invalidate();
    this->cache_.invalidate();
  }

  template <class Data>
  inline text::Face* Label<Data>::font_face() const noexcept
  {
    return this->font_face_;
  }

  /*!
   * \brief Sets the rasterizer implementation to use.
   *
   * \post Invalidates the cache if the passed in rasterizer is different from
   * the one already stored.
   */
  template <class Data>
  inline void Label<Data>::rasterizer(text::Rasterizer* rasterizer) noexcept
  {
    if(this->rasterizer_ == rasterizer) return;
    this->rasterizer_ = rasterizer;
    this->cache_.template grab_dependency<0>().invalidate();
    this->cache_.invalidate();
  }

  template <class Data>
  inline void Label<Data>::mode(Label_Mode m) noexcept
  {
    this->mode_ = m;
  }
  template <class Data>
  inline Label_Mode Label<Data>::mode() const noexcept
  {
    return this->mode_;
  }

  /*!
   * \brief Returns the font renderer.
   */
  template <class Data>
  inline text::Rasterizer* Label<Data>::rasterizer() const noexcept
  {
    return this->rasterizer_;
  }

  template <class Data>
  Label_Cache make_label_cache(Label<Data>& l)
  {
    Label_Cache c;
    c.gen_func([](Label_Cache::ptr_type p,
                  Rasterized_String_Cache& s, SDL_Renderer*& r)
    {
      if(p) return p;
      p.reset(SDL_CreateTextureFromSurface(r, s.cache()->surface));
      return p;
    });

    c.grab_dependency<0>().gen_func(
    [&l](Rasterized_String_Cache::ptr_type p)
    {
      if(p) return p;
      std::string text = boost::lexical_cast<std::string>(l.data());

      // Rasterize all the glyphs first.
      std::vector<text::Rasterized_Glyph> glyphs;
      for(char c : text)
      {
        auto glyph = l.rasterizer()->rasterize(*l.font_face(), l.text_height(),
                                               c, l.text_color());
        glyphs.emplace_back(std::move(glyph));
      }

      // Calculate bounds
      int width = 0;
      int height = 0;
      int baseline = 0;
      {
        int max_ascent = 0;
        int max_descent = 0;
        for(int glyph_indice = 0; glyph_indice < glyphs.size(); ++glyph_indice)
        {
          auto& glyph = glyphs[glyph_indice];
          width += glyph.glyph->root.advance.x >> 16;

          if(glyph_indice != 0)
          {
            width += l.font_face()->kerning(text[glyph_indice - 1],
                                            text[glyph_indice]);
          }

          max_ascent = std::max(max_ascent, glyph.glyph->top);
          max_descent = std::max(max_descent,
                                 glyph.glyph->bitmap.rows - glyph.glyph->top);
        }
        height = max_ascent + max_descent;
        baseline = max_ascent;
      }

      // Blit the text onto the surface.
      text::Unique_Surface line_surf(SDL_CreateRGBSurface(0, width, height,
                                                          32,
                                                          0xff000000,
                                                          0x00ff0000,
                                                          0x0000ff00,
                                                          0x000000ff));

      int pen_x = 0;
      for(int glyph_indice = 0; glyph_indice < glyphs.size(); ++glyph_indice)
      {
        auto& glyph = glyphs[glyph_indice];

        // Figure out kerning if necessary.
        if(glyph_indice != 0)
          pen_x += l.font_face()->kerning(text[glyph_indice- 1],
                                          text[glyph_indice]);

        SDL_Rect dest;
        dest.x = pen_x + glyph.glyph->left;
        dest.y = baseline - glyph.glyph->top;

        SDL_BlitSurface(glyph.surface.get(), NULL, line_surf.get(), &dest);

        pen_x += glyph.glyph->root.advance.x >> 16;
      }

      Rasterized_String str;
      str.baseline = baseline;
      str.surface = line_surf.release();
      return Rasterized_String_Cache::ptr_type(new Rasterized_String(str));
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
   * \param rasterizer The implementation to use when rasterizing text!
   */
  template <class Data>
  Label<Data>::Label(const Data& data,
                     int text_height,
                     math::vector<int> pos,
                     SDL_Color text_color,
                     text::Face* face,
                     text::Rasterizer* rasterizer,
                     Label_Mode mode) noexcept :
                     data_(data),
                     text_height_(text_height),
                     pos_(pos),
                     text_color_(text_color),
                     font_face_(face),
                     rasterizer_(rasterizer),
                     mode_(mode),
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
                     font_face_(label.font_face_),
                     rasterizer_(label.rasterizer_),
                     mode_(label.mode_),
                     cache_(label.cache_) {}
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
                     font_face_(label.font_face_),
                     rasterizer_(label.rasterizer_),
                     mode_(label.mode_),
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
    this->font_face(label.font_face_);
    this->rasterizer(label.rasterizer_);

    this->mode_ = label.mode_;

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
    this->font_face(label.font_face_);
    this->rasterizer(label.rasterizer_);

    this->mode(label.mode_);

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

    cache_.template set_dependency<1>(renderer);
    if(!cache_.cache()) return;


    SDL_Rect dest;
    dest.x = this->pos_.x;
    dest.y = this->pos_.y;

    if(mode_ == Label_Mode::Baseline)
    {
      Rasterized_String* str = cache_.template grab_dependency<0>().cache();
      if(!str) return;
      dest.y -= str->baseline;
    }

    dest.w = this->surface_width();
    dest.h = this->surface_height();
    SDL_RenderCopy(renderer, cache_.cache(), NULL, &dest);
  }
}
