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
                               cache_(cache.cache_)
  {
    cache.cache_ = nullptr;
  }
  CachedSurface& CachedSurface::operator=(CachedSurface&& cache)
  {
    //Free our cache to avoid losing the point to allocated memory.
    this->freeCache();

    //Set our cache to the other cache.
    this->cache_ = cache.cache_;

    //Make sure the other object doesn't free it's cache. Since we are using it.
    //Notice we are not `CachedSurface::freeCache()`ing the surface though,
    //since that would uninitialize the memory.
    cache.cache_ = nullptr;
  }

  void CachedSurface::generateCache() const
  {
    //Don't generate the cache unless we actually have to. That is only if the
    //cache is a nullptr.
    if(!this->cache_)
    {
      this->cache_ = this->generateCache_private();
    }
  }
  void CachedSurface::invalidateCache() const
  {
    //Free the surface to let other functions know it needs to be regenerated.
    //Note: it isn't the freeCache function which signals an invalidation, it's
    //actually the cache being evaluated to false later on. That's why freeing
    //the cache works like we want it too.
    this->freeCache();
  }

  SDL_Surface* CachedSurface::cache() const
  {
    this->generateCache();
    return this->cache_;
  }

  void CachedSurface::freeCache() const
  {
    //Only SDL_FreeSurface the cache if it points to allocated memory.
    if(this->cache_)
    {
      SDL_FreeSurface(this->cache_);
      this->cache_ = nullptr;
    }
  }
};
