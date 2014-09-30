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
#pragma once

#include <unordered_map>
#include <functional>
#include <memory>

#include "SDL.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include "sdl_cache.hpp"
#include "common/vector.h"
#include "server/Logger.h"
namespace pong { namespace text
{
  namespace detail
  {
    struct Library_Deleter
    {
      inline void operator()(FT_Library* l) noexcept
      {
        FT_Done_FreeType(*l);
        delete l;
      }
    };
    using Unique_Library = std::unique_ptr<FT_Library, Library_Deleter>;

    struct Face_Deleter
    {
      inline void operator()(FT_Face* f) noexcept
      {
        FT_Done_Face(*f);
        delete f;
      }
    };
    using Unique_Face = std::unique_ptr<FT_Face, Face_Deleter>;
  }

  namespace detail
  {
    struct Glyph_Cache
    {
      Glyph_Cache(FT_Face* face) noexcept : face_(face) {}
      ~Glyph_Cache() noexcept;

      FT_Glyph glyph(int pixel_size, char c) noexcept;
    private:
      FT_Face* face_;

      // For every pixel size we have maps from glyph index to glyph data.
      std::unordered_map<int, std::unordered_map<char, FT_Glyph> > cache_;

      FT_Glyph load_glyph_(int, char) const noexcept;
      void unload_glyph_(FT_Glyph) const noexcept;
    };
  }
  struct Face
  {
    Face(std::string const&, Logger*) noexcept;
    ~Face() noexcept = default;

    Face(Face const&) noexcept = delete;
    Face& operator=(Face const&) noexcept = delete;

    inline FT_Glyph glyph(int size, char c) noexcept
    { return cache_.glyph(size, c); }

    int kerning(char g1, char g2) noexcept;

    // Implicit move.
  private:
    // Member construction order important!
    detail::Unique_Library library_;
    detail::Unique_Face face_;

    detail::Glyph_Cache cache_;

    Logger* log_;
  };

  struct Metrics
  {
    int ascent, descent;
    math::vector<int> bitmap_extent;
    int bearing, advance;
  };
  Metrics metrics(FT_Glyph glyph) noexcept;

  /*!
   * \brief A unique_ptr which deletes an SDL_Surface* properly.
   */
  using Unique_Surface = std::unique_ptr<SDL_Surface, Surface_Deleter>;

  // Rasterize me... (Doctor who anyone?)
  struct Rasterizer
  {
    virtual ~Rasterizer() noexcept = default;
    virtual Unique_Surface rasterize(FT_Glyph glyph,
                                     SDL_Color color) const noexcept = 0;
  };
  struct MonoRaster : Rasterizer
  {
    Unique_Surface rasterize(FT_Glyph glyph,
                             SDL_Color color) const noexcept override;
  };
  struct AntiAliasedRaster : Rasterizer
  {
    Unique_Surface rasterize(FT_Glyph glyph,
                             SDL_Color color) const noexcept override;
  };
} }
