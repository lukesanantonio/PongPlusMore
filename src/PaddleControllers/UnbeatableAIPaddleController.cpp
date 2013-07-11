#include "UnbeatableAIPaddleController.h"
#include "Paddle.h"
namespace pong
{
  void UnbeatableAIPaddleController::update(Paddle* paddle)
  {
    if(!this->tracked_ball) return;

    math::vector new_position = paddle->position();

    auto ball_center_x = this->tracked_ball->position().x +
                         this->tracked_ball->diameter() / 2;
    new_position.x = ball_center_x - paddle->width() / 2;
    paddle->position(new_position);
  }
};
