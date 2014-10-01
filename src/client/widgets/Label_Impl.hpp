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
    if(dep) return dep->w;
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
    if(dep) return dep->h;
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
  /*!
   * \brief Returns the font renderer.
   */
  template <class Data>
  inline text::Rasterizer* Label<Data>::rasterizer() const noexcept
  {
    return this->rasterizer_;
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

      // Separate the text into lines.
      std::vector<std::string> lines;
      {
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
      }

      // Rasterize all the glyphs but record their metrics.
      std::vector<std::vector<std::pair<text::Metrics,
                                        text::Unique_Surface> > > glyphs;
      glyphs.reserve(lines.size());
      for(std::string line : lines)
      {
        glyphs.emplace_back();
        glyphs.back().reserve(line.size());
        for(char c : line)
        {
          auto glyph = l.font_face()->glyph(l.text_height(), c);
          auto m = text::metrics(glyph);
          glyphs.back().emplace_back(m,
                             l.rasterizer()->rasterize(glyph, l.text_color()));
        }
      }

      // Make lines
      std::vector<text::Unique_Surface> line_surfs;
      line_surfs.reserve(lines.size());
      for(int i = 0; i < lines.size(); ++i)
      {
        std::string const& line = lines[i];

        text::Bitmap_Metrics bm(line, l.text_height(), *l.font_face());
        text::Unique_Surface line_surf(SDL_CreateRGBSurface(0,
                                                            bm.extent.x,
                                                            bm.extent.y,
                                                            32,
                                                            0xff000000,
                                                            0x00ff0000,
                                                            0x0000ff00,
                                                            0x000000ff));

        int pen_x = 0;
        for(auto& glyph : glyphs[i])
        {
          SDL_Rect dest;
          dest.x = pen_x + std::get<0>(glyph).bearing;
          dest.y = bm.baseline - std::get<0>(glyph).ascent;
          SDL_BlitSurface(std::get<1>(glyph).get(), NULL,
                          line_surf.get(), &dest);

          pen_x += std::get<0>(glyph).advance;
        }
        line_surfs.push_back(std::move(line_surf));
      }

      return std::move(line_surfs.back());
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
                     text::Rasterizer* rasterizer) noexcept :
                     data_(data),
                     text_height_(text_height),
                     pos_(pos),
                     text_color_(text_color),
                     font_face_(face),
                     rasterizer_(rasterizer),
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
                     font_face_(label.font_face_),
                     rasterizer_(label.rasterizer_),
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
