/*!
 * \file CachedSurface.cpp
 * \brief Definitions for the base class for renderable objects managing a
 * cache.
 */
#include "CachedSurface.h"
namespace pong
{
  CachedSurface::~CachedSurface()
  {
    //Free the cache so we don't leak memory.
    this->freeCache();
  }

  CachedSurface::CachedSurface(CachedSurface&& cache) :
                               cache_(cache.cache_) //Initialize our cache with
                                                    //their cache...
  {
    cache.cache_ = nullptr;
  }
  CachedSurface& CachedSurface::operator=(CachedSurface&& cache)
  {
    //Free our cache to avoid losing the point to allocated memory.
    this->freeCache();

    //Set our cache to the other cache.
    this->cache_ = cache.cache_;

    //Make sure the other object doesn't free its cache. Since we are using it.
    //Notice we are not `CachedSurface::freeCache()`ing the surface though,
    //since that would free the precious memory!
    cache.cache_ = nullptr;

    return *this;
  }

  /*!
   * \brief Generates the cache on demand.
   *
   * If the cache doesn't require anymore generation, no generation occurs.
   * Feel free to use this function generously.
   */
  void CachedSurface::generateCache() const
  {
    //Don't generate the cache unless we actually have to. That is only if the
    //cache is a nullptr.
    if(!this->cache_)
    {
      this->cache_ = this->generateCache_private();
    }
  }

  /*!
   * \brief Sets the cache to be regenerated next time it is required.
   *
   * \note Currently this function is implemented by `SDL_FreeSurface()`ing
   * the cache on the spot.
   */
  void CachedSurface::invalidateCache() const
  {
    //Free the surface to let other functions know it needs to be regenerated.
    //Note: it isn't the freeCache function which signals an invalidation, it's
    //actually the cache being evaluated to false later on. That's why freeing
    //the cache works like we want it too.
    this->freeCache();
  }

  /*!
   * \brief Returns the generated cache.
   *
   * If we haven't generated it yet, it is generated on the spot. If
   * generation is expensive you can pick the optimal time to generate and
   * call the CachedSurface::generateCache() function.
   */
  SDL_Surface* CachedSurface::cache() const
  {
    this->generateCache();
    return this->cache_;
  }

  /*!
   * \brief `SDL_FreeSurface()`s `this->cache_` *if necessary*.
   *
   * Basically a housekeeping function to avoid code duplication. There really
   * isn't a specific purpose in mind here.
   */
  void CachedSurface::freeCache() const
  {
    //Only free the cache if it points to allocated memory.
    if(this->cache_)
    {
      SDL_FreeSurface(this->cache_);
      this->cache_ = nullptr;
    }
  }
};
