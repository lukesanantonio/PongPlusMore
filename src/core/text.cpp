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
#include "text.h"
#include <vector>
#include <algorithm>
namespace pong { namespace text
{
  namespace detail
  {
    Glyph_Cache::~Glyph_Cache() noexcept
    {
      for(auto& glyph_cache : cache_)
      {
        for(auto& glyph_pair : std::get<1>(glyph_cache))
        {
          unload_glyph_(std::get<1>(glyph_pair));
        }
      }
    }

    FT_Glyph Glyph_Cache::glyph(int size, char c) noexcept
    {
      auto& glyph_cache = cache_[size];
      if(glyph_cache.find(c) == glyph_cache.end())
      {
        glyph_cache.emplace(c, load_glyph_(size, c));
      }
      return glyph_cache[c];
    }

    FT_Glyph Glyph_Cache::load_glyph_(int size, char c) const noexcept
    {
      // Load the glyph.
      FT_Set_Pixel_Sizes(*face_, size, 0);
      FT_Load_Char(*face_, c, 0);

      // Copy the glyph.
      FT_Glyph glyph;
      FT_Get_Glyph((*face_)->glyph, &glyph);
      return glyph;
    }
    void Glyph_Cache::unload_glyph_(FT_Glyph glyph) const noexcept
    {
      FT_Done_Glyph(glyph);
    }
  }

  Face::Face(std::string const& filename, Logger* l) noexcept
             : library_(new FT_Library), face_(new FT_Face),
               cache_(face_.get()), log_(l)
  {
    if(FT_Init_FreeType(library_.get()))
    {
      if(log_) log_->log(Severity::Error, "Failed to init freetype library");
    }
    if(FT_New_Face(*library_, filename.c_str(), 0, face_.get()))
    {
      if(log_) log_->log(Severity::Error, "Failed to init font face object");
    }
  }

  int Face::kerning(char c1, char c2) noexcept
  {
    if(FT_HAS_KERNING((*face_)))
    {
      FT_UInt g1 = FT_Get_Char_Index(*face_, c1);
      FT_UInt g2 = FT_Get_Char_Index(*face_, c2);

      FT_Vector vec;
      FT_Get_Kerning(*face_, g1, g2, 0, &vec);

      return vec.x >> 6;
    }
    return 0;
  }

  Metrics metrics(FT_Glyph glyph) noexcept
  {
    Metrics metrics;

    FT_BBox glyph_box;
    FT_Glyph_Get_CBox(glyph, FT_GLYPH_BBOX_PIXELS, &glyph_box);

    metrics.ascent = glyph_box.yMax;
    metrics.descent = -glyph_box.yMin;

    metrics.bitmap_extent.x = glyph_box.xMax - glyph_box.xMin;
    metrics.bitmap_extent.y = glyph_box.yMax - glyph_box.yMin;

    metrics.bearing = glyph_box.xMin;
    metrics.advance = glyph->advance.x >> 16;

    return metrics;
  }

  Bitmap_Metrics::Bitmap_Metrics(std::string const& str, int size,
                                 Face& f) noexcept
  {
    std::vector<FT_Glyph> glyphs;
    for(char c : str)
    {
      glyphs.push_back(f.glyph(size, c));
    }

    using std::begin; using std::end;
    *this = bitmap_metrics(begin(glyphs), end(glyphs));
  }

  FT_BitmapGlyph MonoRaster::make_bitmap_glyph_(FT_Glyph glyph) const noexcept
  {
    // Rasterize
    FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph) glyph;
    FT_Glyph_To_Bitmap((FT_Glyph*) &bitmap_glyph, FT_RENDER_MODE_MONO, 0, 0);
    return bitmap_glyph;
  }
  Unique_Surface MonoRaster::rasterize_(FT_BitmapGlyph glyph,
                                        SDL_Color color) const noexcept
  {
    // Create surface
    Metrics m = metrics((FT_Glyph) glyph);
    Unique_Surface surf(SDL_CreateRGBSurface(0, m.bitmap_extent.x,
                                             m.bitmap_extent.y, 32,
                                             0xff000000,
                                             0x00ff0000,
                                             0x0000ff00,
                                             0x000000ff));

    // Blit
    uint32_t pix_data = (color.r << 24)|(color.g << 16)|(color.b << 8)|0xff;

    FT_Bitmap* bitmap = &glyph->bitmap;
    //Each pixel, a full byte, will only use it's first bit. A horrible
    //waste of memory, but I'm not sure if I can create an SDL_Surface*
    //with 8 pixels per byte.
    for(int row = 0; row < surf->h; ++row)
    {
      uint8_t* out_row = (uint8_t*) surf->pixels + (row * surf->pitch);
      for(int pix = 0; pix < surf->w; ++pix)
      {
        uint32_t* out = (uint32_t*) out_row + pix;
        uint8_t* src = bitmap->buffer + (row * bitmap->pitch + (pix / 8));

        *out = pix_data;

        bool on = (*src) & (1 << (7 - pix % 8));
        if(on) *out &= 0xffffffff;
        else   *out &= 0xffffff00;
      }
    }

    // Return
    return surf;
  }

  FT_BitmapGlyph
  AntiAliasedRaster::make_bitmap_glyph_(FT_Glyph glyph) const noexcept
  {
    // Rasterize
    FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph) glyph;
    FT_Glyph_To_Bitmap((FT_Glyph*) &bitmap_glyph, FT_RENDER_MODE_NORMAL, 0, 0);
    return bitmap_glyph;
  }

  Unique_Surface AntiAliasedRaster::rasterize_(FT_BitmapGlyph glyph,
                                               SDL_Color color) const noexcept
  {
    // Create surface
    Metrics m = metrics((FT_Glyph) glyph);
    Unique_Surface surf(SDL_CreateRGBSurface(0, m.bitmap_extent.x,
                                             m.bitmap_extent.y,
                                             32,
                                             0xff000000,
                                             0x00ff0000,
                                             0x0000ff00,
                                             0x000000ff));

    // Blit
    FT_Bitmap* bitmap = &glyph->bitmap;
    SDL_LockSurface(surf.get());
    {
      for(int row = 0; row < surf->h; ++row)
      {
        uint8_t* out_row = (uint8_t*) surf->pixels + (row * surf->pitch);
        for(int pix = 0; pix < surf->w; ++pix)
        {
          uint32_t* out = (uint32_t*) out_row + pix;
          uint8_t* src = bitmap->buffer + (row * bitmap->pitch + pix);

          *out |= *src;

          *out |= (0xff & color.r) << 24;
          *out |= (0xff & color.g) << 16;
          *out |= (0xff & color.b) << 8;
        }
      }
    }
    SDL_UnlockSurface(surf.get());

    // Return
    return surf;
  }
} }
