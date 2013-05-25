/*!
 * \file Paddle.h
 * \brief Declares the Paddle class and a PaddleSignals class for events
 * (probably).
 */
#ifndef ULTIMATE_PONG_PADDLE_H
#define ULTIMATE_PONG_PADDLE_H
#include <SDL/SDL.h>
#include "vector.hpp"
namespace pong
{
  /*!
   * \brief A concrete (base) class for paddles.
   */
  class Paddle
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

  private:
    math::vector pos_;
  };
};
#endif