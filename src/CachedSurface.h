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
