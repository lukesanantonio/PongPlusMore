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
namespace pong
{
  /*!
   * \brief Class wrapping renderable text.
   */
  class Label : public Surface_Cache
  {
  public:
    explicit Label(const std::string& text = "",
                   int text_height = 24,
                   math::vector<int> pos = math::vector<int>(),
                   FontRenderer* font_renderer = nullptr) noexcept;

    Label(const std::string& text,
          int text_height,
          math::vector<int> pos,
          SDL_Color text_color,
          SDL_Color back_color,
          FontRenderer* font_renderer) noexcept;

    /*!
     * \brief Free's the text cache if necessary.
     */
    ~Label() noexcept = default;

    Label(const Label&) noexcept;
    Label(Label&&) noexcept;

    Label& operator=(const Label&) noexcept;
    Label& operator=(Label&&) noexcept;

    void render(SDL_Renderer* renderer) const;
    void render(SDL_Surface* surface) const;

    inline int getSurfaceWidth() const;
    inline int getSurfaceHeight() const;

    inline void text(const std::string& text) noexcept;
    inline std::string text() const noexcept;

    inline void text_height(int text_height) noexcept;
    inline int text_height() const noexcept;

    inline void position(math::vector<int> pos) noexcept;
    inline math::vector<int> position() const noexcept;

    inline void text_color(SDL_Color text_color) noexcept;
    inline SDL_Color text_color() const noexcept;

    inline void back_color(SDL_Color back_color) noexcept;
    inline SDL_Color back_color() const noexcept;

    inline void font_renderer(FontRenderer* font_renderer) noexcept;
    inline FontRenderer* font_renderer() const noexcept;
  private:
    /*!
     * \brief The text of the label.
     *
     * \sa Label::text_height_
     */
    std::string text_;

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
     * \brief The background color, (anything not text) in the SDL_Surface
     * cached and rendered.
     */
    SDL_Color back_color_;

    /*!
     * \brief The fontrenderer implementation...
     */
    FontRenderer* font_renderer_ = nullptr;

    virtual ptr_type generateCache_private() const override;
  };

  /*!
   * \brief Returns the width of the cached text surface.
   *
   * \returns Label::cache()->w
   *
   * \post Generates the cache if necessary.
   * \warning This function requires a valid FontRenderer, otherwise it
   * crashes.
   */
  inline int Label::getSurfaceWidth() const
  {
    return this->cache()->w;
  }
  /*!
   * \brief Returns the height of the cached text surface.
   *
   * \returns Label::cache()->h
   *
   * \post Generates the cache if necessary.
   * \warning This function requires a valid FontRenderer, otherwise it
   * crashes.
   */
  inline int Label::getSurfaceHeight() const
  {
    return this->cache()->h;
  }

  /*!
   * \brief Sets the text of the label.
   *
   * \post Invalidates the surface.
   */
  inline void Label::text(const std::string& text) noexcept
  {
    this->text_ = text;
    this->invalidateCache();
  }
  /*!
   * \brief Gets the text of the label.
   *
   * \returns Label::text_
   */
  inline std::string Label::text() const noexcept
  {
    return this->text_;
  }

  /*!
   * \brief Sets the text height of the label.
   *
   * \post Invalidates the surface.
   */
  inline void Label::text_height(int text_height) noexcept
  {
    this->text_height_ = text_height;
    this->invalidateCache();
  }
  /*!
   * \brief Returns the text height of the label.
   *
   * \returns Label::text_height_
   */
  inline int Label::text_height() const noexcept
  {
    return this->text_height_;
  }

  /*!
   * \brief Sets the position of the top left corner of the surface.
   *
   * Does not invalidate the surface.
   */
  inline void Label::position(math::vector<int> pos) noexcept
  {
    this->pos_ = pos;
  }
  /*!
   * \brief Returns the position of the top left corner of the surface.
   *
   * \returns Label::pos_
   */
  inline math::vector<int> Label::position() const noexcept
  {
    return this->pos_;
  }

  /*!
   * \brief Sets the color of the text.
   *
   * \post Invalidates the cache.
   */
  inline void Label::text_color(SDL_Color text_color) noexcept
  {
    this->text_color_ = text_color;
    this->invalidateCache();
  }
  /*!
   * \brief Returns the color of the text.
   *
   * \returns Label::text_color_
   */
  inline SDL_Color Label::text_color() const noexcept
  {
    return this->text_color_;
  }

  /*!
   * \brief Sets the color of the background of the image blitted.
   *
   * \post Invalidates the surface.
   */
  inline void Label::back_color(SDL_Color back_color) noexcept
  {
    this->back_color_ = back_color;
    this->invalidateCache();
  }
  /*!
   * \brief Returns the color of the background of the image blitted.
   *
   * \returns Label::back_color_
   */
  inline SDL_Color Label::back_color() const noexcept
  {
    return this->back_color_;
  }

  /*!
   * \brief Sets the font renderer implementation to use.
   *
   * \post Invalidates the cache.
   */
  inline void Label::font_renderer(FontRenderer* font_renderer) noexcept
  {
    this->font_renderer_ = font_renderer;
    this->invalidateCache();
  }
  /*!
   * \brief Returns the font renderer.
   */
  inline FontRenderer* Label::font_renderer() const noexcept
  {
    return this->font_renderer_;
  }
};
