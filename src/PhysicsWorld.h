#ifndef ULTIMATE_PONG_PHYSICS_WORLD_H
#define ULTIMATE_PONG_PHYSICS_WORLD_H
#include <vector>
#include <memory>
#include "Ball.h"
#include "Paddle.h"
#include "PaddleController.hpp"
namespace pong
{
  struct BallWrapper
  {
    Ball* ball;
    math::vector velocity;
  };

  struct PaddleWrapper
  {
    Paddle* paddle;
    /*!
     * A smart pointer because it is likely to be the only handle to the
     * controller at most times. We avoid destruction this way.
     */
    std::shared_ptr<PaddleController> controller;
  };

  class PhysicsWorld
  {
  public:
    virtual ~PhysicsWorld() noexcept = default;
    /*!
     * \brief Adds a ball to the simulation.
     *
     * \param ball A pointer to the ball.
     * \param velocity The initial velocity of the ball.
     */
    void addBall(Ball* ball, math::vector velocity);
    /*!
     * \brief Adds a paddle as a collision mass
     *
     * Paddle's will be modified, but not necessarily by their respective
     * controller, the PhysicsWorld has full discretion.
     */
    void addPaddle(Paddle* paddle,
                   std::shared_ptr<PaddleController> controller);

    /*!
     * \brief Step the physics, modify all balls in the PhysicsWorld::balls_
     * vector, and update all paddles.
     */
    virtual void step() = 0;
  protected:
    std::vector<BallWrapper> balls_;
    std::vector<PaddleWrapper> paddles_;
  };
};
#endif
