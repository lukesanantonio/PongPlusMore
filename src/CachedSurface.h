/*!
 * \file CachedSurface.h
 * \brief Declarations for the base class for renderable objects managing a
 * cache.
 */
#ifndef ULTIMATE_PONG_CACHED_SURFACE_H
#define ULTIMATE_PONG_CACHED_SURFACE_H
#include <SDL/SDL.h>
namespace pong
{
  /*!
   * \brief Implements a caching system for an SDL_Surface.
   */
  class CachedSurface
  {
  public:
    CachedSurface() = default;
    virtual ~CachedSurface();
    CachedSurface(const CachedSurface&) = delete;
    CachedSurface(CachedSurface&&);
    CachedSurface& operator=(const CachedSurface&) = delete;
    CachedSurface& operator=(CachedSurface&&);

    /*!
     * \brief Generates the cache on demand.
     *
     * If the cache doesn't require anymore generation, no generation occurs.
     * Feel free to use this function generously.
     */
    void generateCache() const;

    /*!
     * \brief Sets the cache to be regenerated next time it is required.
     *
     * \note Currently this function is implemented by `SDL_FreeSurface()`ing
     * the cache on the spot.
     */
    void invalidateCache() const;
  protected:
    /*!
     * \brief Returns the generated cache.
     *
     * If we haven't generated it yet, it is generated on the spot. If
     * generation is expensive you can pick the optimal time to generate and
     * call the CachedSurface::generateCache() function.
     */
    SDL_Surface* cache() const;
  private:
    /*!
     * \brief Should be implemented to return the new SDL_Surface*
     */
    virtual SDL_Surface* generateCache_private() const = 0;

    /*!
     * The cache, nullptr if it needs to be (re)generated.
     */
    mutable SDL_Surface* cache_ = nullptr;

    /*!
     * \brief `SDL_FreeSurface()`s `this->cache_` *if necessary*.
     *
     * Basically a housekeeping function to avoid code duplication. There really
     * isn't a specific purpose in mind here.
     */
    void freeCache() const;
  };
};
#endif
