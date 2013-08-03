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

    inline std::size_t width() const noexcept;
    inline void width(std::size_t width) noexcept;

    inline std::size_t height() const noexcept;
    inline void height(std::size_t height) noexcept;
  private:
    /*!
     * \brief The position of the top left corner of the paddle.
     */
    math::vector pos_;

    /*!
     * \brief Width of the paddle.
     */
    std::size_t width_ = 120;
    /*!
     * \brief Height of the paddle.
     */
    std::size_t height_ = 20;
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
  inline std::size_t Paddle::width() const noexcept
  {
    return this->width_;
  }
  /*!
   * \brief Sets the width of the paddle for future rendering.
   */
  inline void Paddle::width(std::size_t width) noexcept
  {
    this->width_ = width;
  }

  /*!
   * \brief Returns the height of the paddle.
   *
   * \returns Paddle::height_
   */
  inline std::size_t Paddle::height() const noexcept
  {
    return this->height_;
  }
  /*!
   * \brief Sets the height of the paddle for future rendering.
   */
  inline void Paddle::height(std::size_t height) noexcept
  {
    this->height_ = height;
  }
};
