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
 * \file render_text.h
 * \brief File declaring font related procedures as well as free functions with
 * the purpose of aiding SDL_Surface* creation.
 *
 * This filename is quite misleading, but at this point... eh.
 */
#pragma once
#include <memory>
#include "SDL.h"
#include <ft2build.h>
#include FT_FREETYPE_H
namespace pong
{
  /*!
   * \brief A simple deleter which frees `SDL_Surface`s - for use with smart
   * pointers.
   */
  struct SurfaceDeleter
  {
    inline void operator()(SDL_Surface* surface)
    {
      SDL_FreeSurface(surface);
    }
  };
  /*!
   * \brief A unique_ptr which deletes an SDL_Surface* properly.
   */
  using UniquePtrSurface = std::unique_ptr<SDL_Surface, SurfaceDeleter>;

  /*!
   * \brief A simple ABC for FontRenderers.
   */
  class FontRenderer
  {
  public:
    FontRenderer();

    //We can't let our members be copied, they're pointers!
    FontRenderer(const FontRenderer&) = delete;
    FontRenderer& operator=(const FontRenderer&) = delete;

    virtual ~FontRenderer();

    /*!
     * \brief Renders text and returns it as an SDL_Surface*.
     *
     * \param text Text to render!
     * \param pixel_size Pixel size of which to render the text at, passed
     * directly into FreeType.
     * \param text_color The color to render the text.
     * \param background_color The color to render the background of the
     * surface.
     *
     * \returns A unique_ptr storing SDL_Surface, it's deleter is configured so
     * that no memory is leaked if it is lost for some reason.
     *
     * \note Crash on error, anything fairly unrecoverable and serious.
     *
     */
    virtual UniquePtrSurface render_text(const std::string& text,
                                         std::size_t pixel_size,
                                         SDL_Color text_color,
                                         SDL_Color background_color) = 0;
  protected:
    FT_Library library_;
    FT_Face    face_;
  };

  /*!
   * \brief Renders text with no anti-aliasing.
   */
  class MonoTextRenderer : public FontRenderer
  {
    virtual UniquePtrSurface render_text(const std::string& text,
                                         std::size_t pixel_size,
                                         SDL_Color text_color,
                                         SDL_Color background_color) override;
  };

  void setupGrayscalePalette(SDL_Surface*& surface, std::size_t num_colors);

  void invertPalette(SDL_Surface*& surface);

  UniquePtrSurface generateRectangle(std::size_t width, std::size_t height,
                                     SDL_Color color);

  /*!
   * \brief Gets the point which should be the location of an object of
   * `object_length` with the bounds which begin at `begin` and go on for
   * `length`.
   *
   * \param begin The location where the line, etc begins.
   * \param length How far this line, etc, goes for, after `begin`.
   * \param object_length The length of the object which needs to be centered.
   *
   * \return The 1d point which should be the position of an object of length
   * `object_length` if it needs to be centered in a line starting at point
   * `begin` going on for `length` amount.
   *
   * \note Example: If begin==100, length==100, and object_length==50. This
   * function returns 125. This would be the x or y point for on object of
   * length 50 which needs to be centered within the bounds 100-200.
   *
   * \note The algorithm is exactly:
   * \code begin + (length / 2) - (object_length / 2) \endcode
   * This is what is returned.
   */
  constexpr int center(int begin, std::size_t length, std::size_t object_length)
  {
    return begin + (length / 2) - (object_length / 2);
  }
};

