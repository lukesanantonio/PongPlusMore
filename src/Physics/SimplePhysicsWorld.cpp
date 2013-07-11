#include "SimplePhysicsWorld.h"
namespace pong
{
  bool checkCollision(const Ball* ball, const Paddle* paddle)
  {
    //Check collisions on the top and bottom of the paddle.
    if(
      (paddle->position().y + paddle->height() > ball->position().y and
       paddle->position().y < ball->position().y)
                            or
      (paddle->position().y + paddle->height() >
                                     ball->position().y + ball->diameter() and
       paddle->position().y < ball->position().y + ball->diameter()))
    {
      //Check both the left and right sides of the ball and make sure at least
      //one side is inside the paddle's left-right bounds. Confused? Check the
      //code.

      if(
         (ball->position().x > paddle->position().x and
          ball->position().x < paddle->position().x + paddle->width())
                            or
         (ball->position().x + ball->diameter() <
                                    paddle->position().x + paddle->width() and
          ball->position().x + ball->diameter() > paddle->position().x))
      {
        //We have a collision!
        return true;
      }

    }
    return false;
  }
  void SimplePhysicsWorld::step()
  {
    for(BallWrapper& ball : this->balls_)
    {
      //Find every square pixel covered by the ray: ball.velocity.
      math::vector normalized_velocity = math::normalize(ball.velocity);
      double length = math::length(ball.velocity);
      math::vector original_position = ball.ball->position();

      //Slowly step through the ray.
      for(double x = 1.0; x <= length; x += .1)
      {
        //Find the position, likely to be inexact, somewhere in the middle of a
        //pixel.
        math::vector new_position = original_position + (x*normalized_velocity);

        //Truncate to find the exact pixel coordinate.
        new_position.x = static_cast<int>(new_position.x);
        new_position.y = static_cast<int>(new_position.y);

        //Set the new position in the ball, we ain't going back, collision or
        //not.
        ball.ball->position(new_position);

        bool done = false;
        for(const Paddle* paddle : this->paddles_)
        {
          //Check collision with each paddle.
          if(checkCollision(ball.ball, paddle))
          {
             //We have a collision, which means it's time to turn around.
             //For now, just flip the y direction.
             ball.velocity.y = -ball.velocity.y;
             done = true;
             break;
          }
        }
        if (done) break;
      }
    }
  }
};
