#ifndef ULTIMATE_PONG_MOUSE_PADDLE_CONTROLLER_HPP
#define ULTIMATE_PONG_MOUSE_PADDLE_CONTROLLER_HPP
#include "PaddleController.hpp"
namespace pong
{
  class MousePaddleController : public PaddleController
  {
  public:
    inline virtual void update(Paddle* paddle)
    {
      math::vector pos = {Game::getInstance()->events.mousePosition().x,
                          paddle->position().y};
      paddle->position(pos);
    }
  };
}
#endif
