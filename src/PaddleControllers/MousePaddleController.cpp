#include "MousePaddleController.h"
#include "Paddle.h"
#include "Game.h"
namespace pong
{
  void MousePaddleController::update(Paddle* paddle)
  {
    //Get the game instance.
    Game* game = Game::getInstance();

    //Position calculation.
    math::vector pos = {
         game->events.mousePosition().x - static_cast<int>(paddle->width()) / 2,
         paddle->position().y};

    //Bounds constraints.
    if(pos.x < 0) pos.x = 0;
    if(pos.x > game->width() - paddle->width())
    {
      pos.x = game->width() - paddle->width();
    }

    //Update position in the paddle.
    paddle->position(pos);
  }
}