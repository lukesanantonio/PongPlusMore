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
#include "sdl_cache.hpp"
#include "common/vector.h"
#include "render_text.h"
#include "util.hpp"
namespace pong
{
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
                   FontRenderer* font_renderer = nullptr) noexcept;

    /*!
     * \brief Free's the text cache if necessary.
     */
    ~Label() noexcept = default;

    Label(const Label&) noexcept;
    Label(Label&&) noexcept;

    Label<Data>& operator=(const Label&) noexcept;
    Label<Data>& operator=(Label&&) noexcept;

    void render(SDL_Renderer* renderer) const;
    void render(SDL_Surface* s) const;

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

    inline void font_renderer(FontRenderer* font_renderer) noexcept;
    inline FontRenderer* font_renderer() const noexcept;
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

    /*!
     * \brief The fontrenderer implementation...
     */
    FontRenderer* font_renderer_;

    mutable Texture_Cache cache_;
  };
}

#include "Label_Impl.hpp"
