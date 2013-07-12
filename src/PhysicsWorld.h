#ifndef ULTIMATE_PONG_PHYSICS_WORLD_H
#define ULTIMATE_PONG_PHYSICS_WORLD_H
#include <vector>
#include "Ball.h"
#include "Paddle.h"
namespace pong
{
  struct BallWrapper
  {
    Ball* ball;
    math::vector velocity;
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
     * \brief Adds a paddle as a collision mass, but not as a rigid body.
     *
     * \note Paddles added with this function, are not modified at all. That is
     * the job of the pong::PaddleController's.
     */
    void addPaddle(const Paddle* paddle);

    /*!
     * \brief Step the physics, and modify all balls in PhysicsWorld::balls_
     * vector.
     */
    virtual void step() = 0;
  protected:
    std::vector<BallWrapper> balls_;
    std::vector<const Paddle*> paddles_;
  };
};
#endif
