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
 * \file Label.h
 * \brief Contains declarations for the Label class.
 */
#pragma once
#include <string>
#include "SDL.h"
#include "common/Cache.h"
#include "../common/vector.h"
#include "../text.h"
#include "../utility.h"
namespace pong
{
  struct Rasterized_String
  {
    SDL_Surface* surface;
    int baseline;
  };

  struct Rasterized_String_Deleter
  {
    void operator()(Rasterized_String* str)
    {
      SDL_FreeSurface(str->surface);
      delete str;
    }
  };

  using Rasterized_String_Cache =
              Cache_With_Deleter<Rasterized_String, Rasterized_String_Deleter>;

  /*!
   * \brief A simple deleter which frees `SDL_Texture`s - for use with smart
   * pointers, etc.
   */
  struct Texture_Deleter
  {
    void operator()(SDL_Texture* texture)
    {
      SDL_DestroyTexture(texture);
    }
  };

  using Label_Cache = Cache_With_Deleter<SDL_Texture, Texture_Deleter,
                                         Rasterized_String_Cache,
                                         SDL_Renderer*>;

    enum class Label_Mode
    {
      Bitmap,
      Baseline,
    };

  /*!
   * \brief Class wrapping renderable text.
   *
   * The reason the label is not similar to the Paddle and Ball interface is
   * because it needs to maintain a cache, should it return a bare reference
   * to a member, it could be changed at any given time.
   */
  template <class Data>
  class Label
  {
  public:
    explicit Label(const Data& data = Data(),
                   int text_height = 24,
                   math::vector<int> pos = math::vector<int>(),
                   SDL_Color text_color = {0xff, 0xff, 0xff, 0xff},
                   text::Face* face = nullptr,
                   text::Rasterizer* rasterizer = nullptr,
                   Label_Mode mode = Label_Mode::Bitmap) noexcept;

    /*!
     * \brief Free's the text cache if necessary.
     */
    ~Label() noexcept = default;

    Label(const Label&) noexcept;
    Label(Label&&) noexcept;

    Label<Data>& operator=(const Label&) noexcept;
    Label<Data>& operator=(Label&&) noexcept;

    void render(SDL_Renderer* renderer) const;

    inline int surface_width() const;
    inline int surface_height() const;

    inline void data(const Data& data) noexcept;
    inline Data data() const noexcept;

    inline void text_height(int text_height) noexcept;
    inline int text_height() const noexcept;

    inline void position(math::vector<int> pos) noexcept;
    inline math::vector<int> position() const noexcept;

    inline void text_color(SDL_Color text_color) noexcept;
    inline SDL_Color text_color() const noexcept;

    inline void font_face(text::Face* face) noexcept;
    inline text::Face* font_face() const noexcept;

    inline void rasterizer(text::Rasterizer* rasterizer) noexcept;
    inline text::Rasterizer* rasterizer() const noexcept;

    inline void mode(Label_Mode) noexcept;
    inline Label_Mode mode() const noexcept;
  private:
    /*!
     * \brief The content of the label.
     *
     * \sa Label::text_height_
     */
    Data data_;

    /*!
     * \brief The height in pixels of the label (text).
     *
     * \sa Label::text_
     */
    int text_height_;

    /*!
     * \brief The position of the top left corner of the label surface.
     *
     * \sa Label::cached_surface_
     */
    math::vector<int> pos_;

    /*!
     * \brief The color of the text in the SDL_Surface generated of the text.
     */
    SDL_Color text_color_;

    text::Face* font_face_;
    /*!
     * \brief The font rasterizer implementation...
     */
    text::Rasterizer* rasterizer_;

    Label_Mode mode_;

    mutable Label_Cache cache_;
  };
}

#include "Label_Impl.hpp"
