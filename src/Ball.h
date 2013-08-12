/*!
 * \file Ball.h
 * \brief File containing the static ball class declarations.
 */
#pragma once
#include "SDL.h"
#include "vector.h"
#include "RenderableEntity.h"
namespace pong
{
  /*!
   * \brief A *static*, square ball which can only render itself.
   */
  class Ball : public RenderableEntity
  {
  public:
    virtual void render(SDL_Surface*) const override;

    inline math::vector position() const noexcept;
    inline void position(math::vector) noexcept;

    inline unsigned short diameter() const noexcept;
    inline void diameter(unsigned short diameter) noexcept;
  private:
    /*!
     * \brief The position of the top left corner of the ball.
     */
    math::vector pos_;

    /*!
     * \brief The diameter of the square ball.
     *
     * Basically the width and height of the ball packed into one value!
     */
    unsigned short diameter_ = 16;
  };

  /*!
   * \brief Returns the position of the top left corner of the ball.
   *
   * \returns Ball::pos_
   */
  inline math::vector Ball::position() const noexcept
  {
    return this->pos_;
  }
  /*!
   * \brief Sets the position of the top left corner of the ball.
   */
  inline void Ball::position(math::vector pos) noexcept
  {
    this->pos_ = pos;
  }

  /*!
   * \brief Returns the diameter of the square ball.
   *
   * \returns Ball::diameter_
   */
  inline unsigned short Ball::diameter() const noexcept
  {
    return this->diameter_;
  }
  /*!
   * \brief Sets the diameter of the square ball.
   */
  inline void Ball::diameter(unsigned short diameter) noexcept
  {
    this->diameter_ = diameter;
  }
};
