#include "PhysicsWorld.h"
namespace pong
{
  void PhysicsWorld::addBall(Ball* ball, math::vector velocity)
  {
    BallWrapper ball_wrapper;

    ball_wrapper.ball = ball;
    ball_wrapper.velocity = velocity;

    this->balls_.push_back(ball_wrapper);
  }
  void PhysicsWorld::addPaddle(Paddle* paddle,
                               std::shared_ptr<PaddleController> controller)
  {
    PaddleWrapper paddle_wrapper;

    paddle_wrapper.paddle = paddle;
    paddle_wrapper.controller = controller;

    this->paddles_.push_back(paddle_wrapper);
  }
};
