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
 * \file sdl_cache.hpp
 * \brief File containing Cache template instantiations for SDL.
 */
#pragma once
#include "common/Cache.hpp"
#include "SDL.h"
namespace pong
{
  /*!
   * \brief A simple deleter which frees `SDL_Surface`s - for use with smart
   * pointers, etc.
   */
  struct SurfaceDeleter
  {
    void operator()(SDL_Surface* surface)
    {
      SDL_FreeSurface(surface);
    }
  };
  using Surface_Cache = Cache<SDL_Surface, SurfaceDeleter>;

  /*!
   * \brief A simple deleter which frees `SDL_Texture`s - for use with smart
   * pointers, etc.
   */
  struct TextureDeleter
  {
    void operator()(SDL_Texture* texture)
    {
      SDL_DestroyTexture(texture);
    }
  };
  using Texture_Cache = Cache<SDL_Texture, TextureDeleter>;
}
