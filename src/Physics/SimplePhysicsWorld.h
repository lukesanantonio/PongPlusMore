#ifndef ULTIMATE_PONG_SIMPLE_PHYSICS_WORLD_H
#define ULTIMATE_PONG_SIMPLE_PHYSICS_WORLD_H
#include "PhysicsWorld.h"
namespace pong
{
  /*!
   * \brief This physics world is the no-frills, basic, nonultimate-pong, like
   * simulation.
   *
   * Not really that great performance wise, time is
   * O(number of paddles * number of balls).
   */
  class SimplePhysicsWorld : public PhysicsWorld
  {
  public:
    virtual void step() override;
  };
};
#endif
