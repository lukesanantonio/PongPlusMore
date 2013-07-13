#ifndef ULTIMATE_PONG_UNBEATABLE_AI_PADDLE_CONTROLLER_H
#define ULTIMATE_PONG_UNBEATABLE_AI_PADDLE_CONTROLLER_H
#include "PaddleController.hpp"
#include "Ball.h"
namespace pong
{
  class UnbeatableAIPaddleController : public PaddleController
  {
  public:
    UnbeatableAIPaddleController(Ball* tracked = nullptr) :
                                                         tracked_ball(tracked){}
    virtual void update(Paddle* paddle);

    /*!
     * \brief The ball which the paddle needs to be constantly on top of.
     *
     * A nullptr ball will be ignored making
     * UnbeatableAIPaddleController::update a no-op.
     */
    Ball* tracked_ball = nullptr;
  };
};
#endif
