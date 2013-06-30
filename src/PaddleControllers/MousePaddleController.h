#ifndef ULTIMATE_PONG_MOUSE_PADDLE_CONTROLLER_H
#define ULTIMATE_PONG_MOUSE_PADDLE_CONTROLLER_H
#include "PaddleController.hpp"
namespace pong
{
  class MousePaddleController : public PaddleController
  {
  public:
    virtual void update(Paddle*) override;
  };
}
#endif
