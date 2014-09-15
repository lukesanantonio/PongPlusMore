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
#include <stdexcept>
#include <string>
#include "common/vector.h"
#include "common/center.hpp"
#include "common/crash.hpp"
#include FT_GLYPH_H
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

    SDL_SetSurfaceBlendMode(image, SDL_BLENDMODE_BLEND);

    return UniquePtrSurface(image);
  }

  UniquePtrSurface GrayscaleTextRenderer::render_text(const std::string& text,
                                                      int pixel_size,
                                                      SDL_Color front_color,
                                                      SDL_Color back_color)
  {
    // Set text sizes.
    FT_Set_Pixel_Sizes(this->face_, pixel_size, 0);

    // Load glyphs.
    struct Glyph
    {
      FT_UInt index;
      FT_Glyph glyph;
    };
    std::vector<Glyph> glyphs;

    for(char c : text)
    {
      Glyph glyph;

      glyph.index = FT_Get_Char_Index(this->face_, c);

      if(FT_Load_Glyph(this->face_, glyph.index, FT_LOAD_DEFAULT))
      {
        throw std::runtime_error("Failed to load glyph.");
      }
      if(FT_Get_Glyph(this->face_->glyph, &glyph.glyph))
      {
        throw std::runtime_error("Failed to 'get' glyph.");
      }

      glyphs.push_back(glyph);
    }

    auto foreach_glyph = [&](auto func)
    {
      bool has_kerning = FT_HAS_KERNING(this->face_);
      int pen_x = 0;
      for(auto iter = glyphs.begin(); iter != glyphs.end(); ++iter)
      {
        // Check possible kerning.
        if(iter != glyphs.begin() && has_kerning)
        {
          FT_Vector delta;
          FT_Get_Kerning(this->face_, (iter - 1)->index, iter->index,
                         FT_KERNING_DEFAULT, &delta);
          pen_x += delta.x >> 6;
        }

        func(*iter, pen_x);

        // Move forward.
        pen_x += iter->glyph->advance.x >> 16;
      }

      return pen_x;
    };

    // Find the extents of the all the glyphs while factoring kerning and such.
    int min_y = 0, max_y = 0;
    int pen_x = foreach_glyph([&min_y, &max_y](Glyph& glyph, int)
    {
      FT_BBox box;
      FT_Glyph_Get_CBox(glyph.glyph, FT_GLYPH_BBOX_PIXELS, &box);

      min_y = std::min<int>(min_y, box.yMin);
      max_y = std::max<int>(max_y, box.yMax);
    });

    // Initialize the final image.
    UniquePtrSurface surface(SDL_CreateRGBSurface(0, pen_x, max_y - min_y,
                                                  8, 0, 0, 0, 0));
    SDL_SetSurfaceBlendMode(surface.get(), SDL_BLENDMODE_BLEND);
    initialize_grayscale_palette(surface.get(), front_color, back_color);

    // Blit the glpyhs onto the final image.
    SDL_LockSurface(surface.get());
    foreach_glyph([&, baseline = max_y](Glyph& glyph, int pen_x)
    {
      if(FT_Glyph_To_Bitmap(&glyph.glyph, FT_RENDER_MODE_NORMAL, 0, 1))
      {
        throw std::runtime_error("Failed to get convert glyph to bitmap.");
      }
      FT_BitmapGlyph bitmap = (FT_BitmapGlyph) glyph.glyph;

      int start_col = pen_x + bitmap->left;
      int start_row = baseline - bitmap->top;

      // Copy data.
      unsigned char* src = bitmap->bitmap.buffer;
      for(int i = 0; i < bitmap->bitmap.rows; ++i)
      {
        for(int j = 0; j < bitmap->bitmap.width; ++j)
        {
          int index = (i + start_row) * surface->pitch + (start_col + j);
          unsigned char* dst = (unsigned char*) surface->pixels;
          unsigned char* src_ptr = src + (i * bitmap->bitmap.pitch + j);
          if(*src_ptr) dst[index] = *src_ptr;
        }
      }
    });
    SDL_UnlockSurface(surface.get());

    // Uninitialize glyphs
    for (Glyph glyph : glyphs)
    {
      FT_Done_Glyph(glyph.glyph);
    }

    return surface;
  }

  double mix(double left, double right, double where)
  {
    return left + (right - left) * where;
  }

  /*!
   * \brief Sets up a grayscale palette for an SDL_Surface.
   */
  void initialize_grayscale_palette(SDL_Surface* surface,
                                    SDL_Color front, SDL_Color back)
  {
    constexpr int size = 256;
    std::array<SDL_Color, size> colors;
    for(int i = 0; i < size; ++i)
    {
      colors[i].r = mix(back.r, front.r, i / 255.0);
      colors[i].g = mix(back.g, front.g, i / 255.0);
      colors[i].b = mix(back.b, front.b, i / 255.0);
      colors[i].a = mix(back.a, front.a, i / 255.0);
    }
    SDL_SetPaletteColors(surface->format->palette, &colors[0], 0, size);
  }
};
