#include "SimplePhysicsWorld.h"
#include <cmath>
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

        new_position.x = static_cast<int>(new_position.x);

        //Truncate to find the exact pixel coordinate. <-- Wrong!
        //Truncation, in some cases, results in some positions being not
        //calculated due to the fact that floating-point rounding errors rarely
        //let the x<=length loop finish with x==length. That is because it is
        //just a little over, making the statement false. Anyway since posiitons
        //are truncated, some positions are never reached. An example: the
        //ball's initial has a velocity of {0,-2}. Since truncation is basically
        //rounding down, expected results occur when the ball is going up but
        //not going down. That is because when going up round down results in
        //the ball prefering the tile it will go into, since the ball is moving
        //up, it makes sense to have the ball prefer the top pixel compared to
        //a bottom pixel. However when the ball bounces having a velocity of
        //{0, 2}, the ball ends up rounding itself to tiles where it has already
        //gone. Resulting in some locations never being reached. This essetially
        //makes the velocity {0,2} appear as {0,1}.
        //
        //To solve this, we floor() the y value when going up (y<0) and ceil()
        //the value when going down (y>0).
        if(normalized_velocity.y > 0)
        {
          new_position.y = ceil(new_position.y);
        }
        //Truncate by default!
        else
        {
          new_position.y = floor(new_position.y);
        }
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
