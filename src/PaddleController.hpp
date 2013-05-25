/*!
 * \file PaddleController.hpp
 * \brief Contains the declaration of the ABC for controlling a paddle.
 */
#ifndef ULTIMATE_PONG_PADDLE_CONTROLLER_HPP
#define ULTIMATE_PONG_PADDLE_CONTROLLER_HPP
#include "Paddle.h"
namespace pong
{
  class Paddle;
  /*!
   * \brief ABC for controlling a paddle. Yay, an abstraction.
   */
  struct PaddleController
  {
  public:
    PaddleController() = default;
    virtual ~PaddleController() = default;
    PaddleController(const PaddleController&) = delete;
    PaddleController(PaddleController&&) = delete;
    PaddleController& operator=(const PaddleController&) = delete;
    PaddleController& operator=(PaddleController&&) = delete;

    /*!
     * \brief Generates a position and updates the Paddle accordingly.
     *
     * \param paddle The paddle which needs an updated position. Will be
     * modified!
     */
    virtual void update(Paddle* paddle) = 0;
  };
};
#endif
