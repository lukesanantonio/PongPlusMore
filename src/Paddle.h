/*!
 * \file Paddle.h
 * \brief Declares the Paddle class and a PaddleSignals class for events
 * (probably).
 */
#ifndef ULTIMATE_PONG_PADDLE_H
#define ULTIMATE_PONG_PADDLE_H
#include <memory>
#include <SDL/SDL.h>
#include "vector.h"
#include "CachedSurface.h"
namespace pong
{
  /*!
   * \brief A concrete (base) class for paddles, though static.
   */
  class Paddle : public CachedSurface
  {
  public:
    Paddle() = default;
    virtual ~Paddle() = default;
    Paddle(const Paddle&) = delete;
    Paddle(Paddle&&) = delete;
    Paddle& operator=(const Paddle&) = delete;
    Paddle& operator=(Paddle&&) = delete;
    /*!
     * \brief Renders the paddle onto the surface at Paddle::pos_.
     */
    void render(SDL_Surface* surface) const;

    /*!
     * \brief Returns the current position of the top left corner of the paddle.
     *
     * \returns Paddle::pos_
     */
    inline math::vector position() const noexcept;
    /*!
     * \brief Sets the position of the top left corner of the paddle when it
     * needs to be rendered.
     *
     * \param pos New position after function returns.
     */
    inline void position(math::vector pos) noexcept;

    /*!
     * \brief Returns the width of the paddle.
     *
     * \returns Paddle::width_
     */
    inline std::size_t width() const noexcept;
    /*!
     * \brief Sets the width of the paddle for future rendering.
     *
     * \note Invalidates the cache.
     */
    inline void width(std::size_t width) noexcept;

    /*!
     * \brief Returns the height of the paddle.
     *
     * \returns Paddle::height_
     */
    inline std::size_t height() const noexcept;
    /*!
     * \brief Sets the height of the paddle for future rendering.
     *
     * \note Invalidates the cache.
     */
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

    virtual SDL_Surface* generateCache_private() const override;
  };

  inline math::vector Paddle::position() const noexcept
  {
    return this->pos_;
  }
  inline void Paddle::position(math::vector pos) noexcept
  {
    this->pos_ = pos;
  }

  inline std::size_t Paddle::width() const noexcept
  {
    return this->width_;
  }
  inline void Paddle::width(std::size_t width) noexcept
  {
    this->width_ = width;
    this->invalidateCache();
  }

  inline std::size_t Paddle::height() const noexcept
  {
    return this->height_;
  }
  inline void Paddle::height(std::size_t height) noexcept
  {
    this->height_ = height;
    this->invalidateCache();
  }
};
#endif
