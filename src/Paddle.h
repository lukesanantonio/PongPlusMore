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
 * \file Paddle.h
 * \brief Declares the Paddle class.
 */
#pragma once
#include <memory>
#include "SDL.h"
#include "vector.h"
#include "RenderableEntity.h"
namespace pong
{
  /*!
   * \brief A concrete (base) class for paddles, though static.
   */
  class Paddle : public RenderableEntity
  {
  public:
    Paddle() = default;
    virtual ~Paddle() = default;
    Paddle(const Paddle&) = delete;
    Paddle(Paddle&&) = delete;
    Paddle& operator=(const Paddle&) = delete;
    Paddle& operator=(Paddle&&) = delete;

    virtual void render(SDL_Surface*) const override;

    inline math::vector position() const noexcept;
    inline void position(math::vector pos) noexcept;

    inline unsigned short width() const noexcept;
    inline void width(unsigned short width) noexcept;

    inline unsigned short height() const noexcept;
    inline void height(unsigned short height) noexcept;
  private:
    /*!
     * \brief The position of the top left corner of the paddle.
     */
    math::vector pos_;

    /*!
     * \brief Width of the paddle.
     */
    unsigned short width_ = 120;
    /*!
     * \brief Height of the paddle.
     */
    unsigned short height_ = 20;
  };

  /*!
   * \brief Returns the current position of the top left corner of the paddle.
   *
   * \returns Paddle::pos_
   */
  inline math::vector Paddle::position() const noexcept
  {
    return this->pos_;
  }
  /*!
   * \brief Sets the position of the top left corner of the paddle when it
   * needs to be rendered.
   *
   * \param pos New position after function returns.
   */
  inline void Paddle::position(math::vector pos) noexcept
  {
    this->pos_ = pos;
  }

  /*!
   * \brief Returns the width of the paddle.
   *
   * \returns Paddle::width_
   */
  inline unsigned short Paddle::width() const noexcept
  {
    return this->width_;
  }
  /*!
   * \brief Sets the width of the paddle for future rendering.
   */
  inline void Paddle::width(unsigned short width) noexcept
  {
    this->width_ = width;
  }

  /*!
   * \brief Returns the height of the paddle.
   *
   * \returns Paddle::height_
   */
  inline unsigned short Paddle::height() const noexcept
  {
    return this->height_;
  }
  /*!
   * \brief Sets the height of the paddle for future rendering.
   */
  inline void Paddle::height(unsigned short height) noexcept
  {
    this->height_ = height;
  }
};
