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
 * \file CachedSurface.h
 * \brief Declarations for the base class for renderable objects managing a
 * cache.
 */
#pragma once
#include "SDL.h"
namespace pong
{
  /*!
   * \brief Implements a caching system for an SDL_Surface.
   *
   * This class is basically an implementation detail, it only works if it's
   * derived from.
   */
  class CachedSurface
  {
  public:
    CachedSurface() = default;
    virtual ~CachedSurface();
    CachedSurface(const CachedSurface&) = delete;
    CachedSurface& operator=(const CachedSurface&) = delete;

    void generateCache() const;
    void invalidateCache() const;
  protected:
    SDL_Surface* cache() const;

    CachedSurface(CachedSurface&&);
    CachedSurface& operator=(CachedSurface&&);
  private:
    /*!
     * \brief Should be implemented to return the new SDL_Surface*
     */
    virtual SDL_Surface* generateCache_private() const = 0;

    /*!
     * The cache, nullptr if it needs to be (re)generated.
     */
    mutable SDL_Surface* cache_ = nullptr;

    void freeCache() const;
  };
};
