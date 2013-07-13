/*!
 * \file SinglePlayerGameState.cpp
 * \brief Contains definitions for the SinglePlayerGameState GameState.
 */
#include "SinglePlayerGameState.h"
#include "Game.h"
#include "PaddleControllers/MousePaddleController.h"
#include "PaddleControllers/UnbeatableAIPaddleController.h"
#include "GameStates/MenuGameState.h"
#include "render_text.h"
#include "Physics/SimplePhysicsWorld.h"
namespace pong
{
  SinglePlayerGameState::SinglePlayerGameState()
  {
    //Initialize paddles (positions, etc).
    this->topPaddle_.position({0, 0});

    this->bottomPaddle_.position(
    //                                ,--This is casted to a double here to
    //                               /   avoid narrowing conversion warnings. I
    //                              \/   don't really like it but, eh.
                    {0,static_cast<double>(Game::getInstance()->height()
                                        - this->bottomPaddle_.height())});

    //Initialize the physics world.
    this->world_.addPaddle(&this->topPaddle_,
                  std::make_shared<UnbeatableAIPaddleController>(&this->ball_));
    this->world_.addPaddle(&this->bottomPaddle_,
                           std::make_shared<MousePaddleController>());

    this->world_.addBall(&this->ball_, {0, 5});

    //Set initial position of the ball. The center.
    math::vector ball_pos;
    ball_pos.x = center(0, Game::getInstance()->width(),
                        this->ball_.diameter());
    ball_pos.y = center(0, Game::getInstance()->height(),
                        this->ball_.diameter());
    this->ball_.position(ball_pos);

    //Configure the mouse.
    SDL_WM_GrabInput(SDL_GRAB_ON);
    SDL_ShowCursor(SDL_DISABLE);
  }

  void SinglePlayerGameState::update_private()
  {
    if(Game::getInstance()->events.keyHasBeenPressed())
    {
      Game::getInstance()->setGameState(std::shared_ptr<GameState>(
                                                            new MenuGameState));
    }
    this->world_.step();
  }
  void SinglePlayerGameState::render_private(SDL_Surface* surface) const
  {
    this->topPaddle_.render(surface);
    this->bottomPaddle_.render(surface);
    this->ball_.render(surface);
  }
};
