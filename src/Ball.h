/*!
 * \file Ball.h
 * \brief File containing the static ball class declarations.
 */
#ifndef ULTIMATE_PONG_BALL_H
#define ULTIMATE_PONG_BALL_H
#include <cstddef> //For std::size_t
#include "CachedSurface.h"
#include "vector.h"
namespace pong
{
  /*!
   * \brief A *static*, square ball which can only render itself.
   */
  class Ball : public CachedSurface
  {
  public:
    void render(SDL_Surface* surface) const;

    /*!
     * \brief Returns the position of the top left corner of the ball.
     *
     * \returns Ball::pos_
     */
    inline math::vector position() const noexcept;
    /*!
     * \brief Sets the position of the top left corner of the ball.
     *
     * Does not invalidate the cache.
     */
    inline void position(math::vector) noexcept;

    /*!
     * \brief Returns the diameter of the square ball.
     *
     * \returns Ball::diameter_
     */
    inline std::size_t diameter() const noexcept;
    /*!
     * \brief Sets the diameter of the sqare ball.
     *
     * Invalidates the cache.
     */
    inline void diameter(std::size_t diameter) noexcept;
  private:
    /*!
     * \brief The position of the top left corner of the ball.
     */
    math::vector pos_;

    /*!
     * \brief The diameter of the square ball. Basically the width and height
     * of the ball.
     */
    std::size_t diameter_ = 16;

    virtual SDL_Surface* generateCache_private() const;
  };
  inline math::vector Ball::position() const noexcept
  {
    return this->pos_;
  }
  inline void Ball::position(math::vector pos) noexcept
  {
    this->pos_ = pos;
  }

  inline std::size_t Ball::diameter() const noexcept
  {
    return this->diameter_;
  }
  inline void Ball::diameter(std::size_t diameter) noexcept
  {
    this->diameter_ = diameter;
    this->invalidateCache();
  }
};
#endif