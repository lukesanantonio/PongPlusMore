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
 * \file render_text.cpp
 * \brief Definitions for everything in render_text.h.
 */
#include "render_text.h"
#include <vector>
#include <algorithm>
#include <string>
#include "common/vector.h"
#include "common/center.hpp"
#include "common/crash.hpp"
namespace pong
{
  /*!
   * \brief Initializes Freetype into FontRenderer::library_ and a face
   * (FontRenderer::face_) ready to go for implementors of FontRenderer.
   */
  FontRenderer::FontRenderer(const std::string& fontfile)
  {
    if(FT_Init_FreeType(&this->library_))
    {
      crash("Failed to initialize FreeType!");
    }
    if(FT_New_Face(this->library_,
                   fontfile.c_str(),
                   0,
                   &this->face_))
    {
      crash("Failed to initialize a FreeType face! Does the font file exist?");
    }
  }
  FontRenderer::~FontRenderer()
  {
    FT_Done_Face(this->face_);
    FT_Done_FreeType(this->library_);
  }
  void runtime_assert_surface(SDL_Surface* sf, int width, int height)
  {
    if(!sf)
    {
      crash("Failed to create an SDL_Surface of size: " +
            std::to_string(width) + "x" + std::to_string(height) + ".");
    }
  }
  UniquePtrSurface MonoTextRenderer::render_text(const std::string& text,
                                                 int pixel_size,
                                                 SDL_Color text_color,
                                                 SDL_Color background_color)
  {
    if(FT_Set_Pixel_Sizes(this->face_, 0, pixel_size))
    {
      crash("Failed to set FreeType pixel size!");
    }

    //Find surface height and surface width.

    int max_top = 0;
    int max_bottom = 0;
    int width = 0;
    for(const char c : text)
    {
      if(FT_Load_Char(this->face_, c, FT_LOAD_RENDER | FT_LOAD_MONOCHROME |
                                      FT_LOAD_TARGET_MONO))
      {
        crash(std::string("Failed to load char '") + c + "' while generating"
        "bounds information for text surface.");
      }

      max_top = std::max(max_top, this->face_->glyph->bitmap_top);
      max_bottom = std::max(max_bottom,
                            this->face_->glyph->bitmap.rows
                            - this->face_->glyph->bitmap_top);
      width += this->face_->glyph->advance.x / 64;
    }

    int height = max_top + max_bottom;

    //Now actually render the string of text.
    SDL_Surface* image = SDL_CreateRGBSurface(SDL_SWSURFACE,
                                              width, height, 8,
                                              0, 0, 0, 0);

    //Make sure we have a valid surface!
    runtime_assert_surface(image, width, height);

    SDL_Color colors[] = {background_color, text_color};
    SDL_SetPaletteColors(image->format->palette, colors, 0, 2);

    int pen_x = 0;
    //Set pen_y to the baseline...
    int pen_y = max_top;
    for(const char c : text)
    {
      if(FT_Load_Char(this->face_, c, FT_LOAD_RENDER | FT_LOAD_MONOCHROME |
                                      FT_LOAD_TARGET_MONO))
      {
        crash(std::string("Failed to load char: '") + c + "'");
      }
      SDL_Surface* glyph = SDL_CreateRGBSurface(
                                              SDL_SWSURFACE,
                                              this->face_->glyph->bitmap.width,
                                              this->face_->glyph->bitmap.rows,
                                              8,
                                              0, 0, 0, 0);

      //Check that surface
      runtime_assert_surface(glyph, this->face_->glyph->bitmap.width,
                                    this->face_->glyph->bitmap.rows);

      SDL_SetPaletteColors(glyph->format->palette, colors, 0, 2);

      SDL_LockSurface(glyph);
      {
        //Each pixel, a full byte, will only use it's first bit. A horrible
        //waste of memory, but I'm not sure if I can create an SDL_Surface*
        //with 8 pixels per byte.
        for(int row = 0; row < glyph->h; ++row)
        {
          for(int pix = 0; pix < glyph->w; ++pix)
          {
            uint8_t* sdlByte =
               static_cast<uint8_t*>(glyph->pixels) + row * glyph->pitch + pix;
            *sdlByte = static_cast<bool>(
                   this->face_->glyph->bitmap.buffer
                           [row * this->face_->glyph->bitmap.pitch + (pix / 8)]
                           & (1 << (7 - pix % 8)));
          }
        }
      }
      SDL_UnlockSurface(glyph);

      //Okay, so we have a solid glyph image, now blit it to the main surface.
      SDL_Rect dest;
      dest.x = pen_x;
      dest.y = pen_y - this->face_->glyph->bitmap_top;
      SDL_BlitSurface(glyph, NULL, image, &dest);

      //Free our temp glyph surface.
      SDL_FreeSurface(glyph);

      //Update pen_x
      pen_x += this->face_->glyph->advance.x / 64;
    }

    return UniquePtrSurface(image);
  }

  /*!
   * \brief Sets up a grayscale palette for an SDL_Surface.
   *
   * \param surface Surface to modify in-place.
   * \param num_colors The amount of shades of gray to generate for the
   * palette.
   * \note For an 8-bit surface, this should not exceed 256.
   */
  void setupGrayscalePalette(SDL_Surface*& surface, int num_colors)
  {
    SDL_Color colors[num_colors];
    for(unsigned int i = 0; i < num_colors; ++i)
    {
      colors[i].r = i;
      colors[i].g = i;
      colors[i].b = i;
    }
    SDL_SetPaletteColors(surface->format->palette, colors, 0, num_colors);
  }

  /*!
   * \brief Inverts the palette of the passed in surface.
   *
   * \param surface Surface whose palette to invert.
   */
  void invertPalette(SDL_Surface*& surface)
  {
    //Make our surface's palette easier to reference.
    SDL_Palette* palette = surface->format->palette;

    //Get a vector of all the colors...
    std::vector<SDL_Color> colors(palette->colors,
                                  palette->colors + palette->ncolors);
    //Reverse our colors.
    std::reverse(colors.begin(), colors.end());

    //Set our palette to the reversed copy of the original.
    SDL_SetPaletteColors(palette, &colors[0], 0, palette->ncolors);
  }

  /*!
   * \brief Generates a rectange SDL_Surface of any color, width, or height.
   *
   * \returns An SDL_Create(d)RGBSurface() of the color passed in.
   * \returns nullptr on error.
   */
  UniquePtrSurface generateRectangle(int width, int height,
                                     SDL_Color color)
  {
    SDL_Surface* surface = SDL_CreateRGBSurface(SDL_SWSURFACE,
                                                width, height, 8,
                                                0, 0, 0, 0);

    runtime_assert_surface(surface, width, height);

    SDL_SetPaletteColors(surface->format->palette, &color, 0, 1);
    SDL_FillRect(surface, nullptr, SDL_MapRGB(surface->format,
                                              color.r,
                                              color.g,
                                              color.b));
    return UniquePtrSurface(surface);
  }
};
