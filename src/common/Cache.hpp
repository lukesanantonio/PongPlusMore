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
 * \file Cache.hpp
 */
#pragma once
#include <memory>
#include <functional>
namespace pong
{
  /*!
   * \brief Implements a caching system for arbitrary pointers.
   */
  template <typename T,
            class D = std::default_delete<T> >
  class Cache
  {
  public:
    /*!
     * \brief Smart pointer type which will manage the cache.
     */
    using ptr_type = std::unique_ptr<T, D>;
    /*!
     * \brief Generation function type.
     */
    using gen_func_type = std::function<ptr_type (ptr_type&&)>;

    /*!
     * \brief Constructs a cache without any particular generation function.
     *
     * If a cache generation is requested while the generation function is
     * still invalid/empty, an exception is thrown. Change that with
     * Cache::gen_func(const gen_func_type&).
     */
    Cache() noexcept : gen_func_() {}
    /*!
     * \brief Constructs a cache with a generation function.
     */
    Cache(gen_func_type f) noexcept : gen_func_(f) {}

    Cache(Cache&&) noexcept;
    Cache& operator=(Cache&&) noexcept;

    Cache(const Cache&) noexcept;
    Cache& operator=(const Cache&) noexcept;

    inline const T* ccache() const noexcept;
    inline T* cache();

    inline bool generate();
    inline void invalidate() noexcept;

    inline gen_func_type gen_func() const noexcept;
    inline void gen_func(const gen_func_type& f) noexcept;
  private:

    /*!
     * \brief Smart pointer managing the cache.
     */
    ptr_type cache_ = nullptr;

    /*!
     * \brief Function used to generate the cache.
     */
    gen_func_type gen_func_;
  };

  /*!
   * \brief Move constructor, moves the cache!
   */
  template <typename T, class D>
  Cache<T, D>::Cache(Cache&& c) noexcept
                                 : cache_(std::move(c.cache_)),
                                   gen_func_(std::move(c.gen_func_)){}

  /*!
   * \brief Move assignment operator, moves the cache!
   */
  template <typename T, class D>
  auto Cache<T, D>::operator=(Cache&& c) noexcept -> Cache&
  {
    this->cache_ = std::move(c.cache_);
    this->gen_func_ = std::move(c.gen_func_);

    return *this;
  }

  /*!
   * \brief Copies the generation function only.
   */
  template <typename T, class D>
  Cache<T, D>::Cache(const Cache& c) noexcept : gen_func_(c.gen_func_) {}

  /*!
   * \brief Copies the generation function only.
   */
  template <typename T, class D>
  auto Cache<T, D>::operator=(const Cache& c) noexcept -> Cache&
  {
    this->gen_func_ = c.gen_func_;

    return *this;
  }

  /*!
   * \brief Returns the cache, but won't generate it at all.
   *
   * \returns The current state of the cache currently (no generation is done.)
   */
  template <typename T, class D>
  inline const T* Cache<T, D>::ccache() const noexcept
  {
    return this->cache_.get();
  }

  /*!
   * \brief Returns the cache, generating it if necessary.
   *
   * A generation occurs if the returned pointer will be a nullptr, if it
   * still is a nullptr after the generation than that is what is returned.
   */
  template <typename T, class D>
  inline T* Cache<T, D>::cache()
  {
    if(!this->cache_) this->cache_ = this->gen_func_(std::move(this->cache_));
    return this->cache_.get();
  }

  /*!
   * \brief Generates the cache possibly factoring in its previous value.
   */
  template <typename T, class D>
  inline auto Cache<T, D>::generate() -> bool
  {
    this->cache_ = this->gen_func_(std::move(this->cache_));
    return this->cache_;
  }

  /*!
   * \brief Invalidates the cache completely.
   *
   * This means it will have to be completely regenerated at some later time.
   */
  template <typename T, class D>
  inline auto Cache<T, D>::invalidate() noexcept -> void
  {
    this->cache_.reset(nullptr);
  }

  /*!
   * \brief Returns the generation function.
   *
   * \returns Cache::gen_func_.
   */
  template <typename T, class D>
  inline auto
  Cache<T, D>::gen_func() const noexcept -> gen_func_type
  {
    return this->gen_func_;
  }

  /*!
   * \brief Sets the generation function of the cache.
   *
   * \param f The new function. If empty this function is a complete no-op.
   * \post Invalidates the cache if f is nonempty.
   */
  template <typename T, class D>
  inline
  void Cache<T, D>::gen_func(const gen_func_type& f) noexcept
  {
    // f is empty? Get out!
    if(!f) return;
    this->invalidate();
    this->gen_func_ = f;
  }
};
