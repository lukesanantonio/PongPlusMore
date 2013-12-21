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
 * \file cache.hpp
 */
#pragma once
#include <memory>
namespace pong
{
  /*!
   * \brief Implements a caching system for arbitrary pointers.
   *
   * This class is basically an implementation detail, it only works if it's
   * derived from.
   */
  template <class Type, class Deleter = std::default_delete<Type> >
  class Cache_Base
  {
  public:
    Cache_Base() = default;
    virtual ~Cache_Base() = default;
    Cache_Base(const Cache_Base&) = delete;
    Cache_Base& operator=(const Cache_Base&) = delete;

    void generateCache() const;
    void invalidateCache() const;
  protected:
    Type* cache() const;

    template <class Type2, class Deleter2>
    Cache_Base(Cache_Base<Type2, Deleter2>&&);

    template <class Type2, class Deleter2>
    Cache_Base& operator=(Cache_Base<Type2, Deleter2>&&);

    using ptr_type = std::unique_ptr<Type, Deleter>;
  private:
    /*!
     * \brief Should be implemented to return an allocated object
     */
    virtual ptr_type generateCache_private() const = 0;

    /*!
     * The cache, nullptr if it needs to be (re)generated.
     */
    mutable ptr_type cache_{nullptr};
  };

  template <class Type1, class Deleter1>
  template <class Type2, class Deleter2>
  Cache_Base<Type1, Deleter1>::Cache_Base(Cache_Base<Type2, Deleter2>&& cache)
                                          : cache_(std::move(cache.cache_)) {}

  template <class Type1, class Deleter1>
  template <class Type2, class Deleter2>
  auto Cache_Base<Type1, Deleter1>::operator=(
                       Cache_Base<Type2, Deleter2>&& cache) -> Cache_Base&
  {
    this->cache_ = std::move(cache.cache_);
    return *this;
  }

  template <class Type, class Deleter>
  void Cache_Base<Type, Deleter>::generateCache() const
  {
    if(!this->cache_) this->cache_ = this->generateCache_private();
  }

  template <class Type, class Deleter>
  void Cache_Base<Type, Deleter>::invalidateCache() const
  {
    this->cache_ = nullptr;
  }

  template <class Type, class Deleter>
  Type* Cache_Base<Type, Deleter>::cache() const
  {
    this->generateCache();
    return this->cache_.get();
  }

};
