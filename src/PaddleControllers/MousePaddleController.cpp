#include "MousePaddleController.h"
#include "Paddle.h"
#include "Game.h"
namespace pong
{
  void MousePaddleController::update(Paddle* paddle)
  {
    Game* game = Game::getInstance();
    math::vector pos = {game->events.mousePosition().x,
                        paddle->position().y};
    if(pos.x < 0) pos.x = 0;
    if(pos.x > game->width() - paddle->width())
    {
      pos.x = game->width() - paddle->width();
    }
    paddle->position(pos);
  }
}