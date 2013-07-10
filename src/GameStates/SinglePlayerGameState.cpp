/*!
 * \file SinglePlayerGameState.cpp
 * \brief Contains definitions for the SinglePlayerGameState GameState.
 */
#include "SinglePlayerGameState.h"
#include "Game.h"
#include "PaddleControllers/MousePaddleController.h"
#include "GameStates/MenuGameState.h"
#include "render_text.h"
#include "Physics/SimplePhysicsWorld.h"
namespace pong
{
  SinglePlayerGameState::SinglePlayerGameState() :world_(new SimplePhysicsWorld)
  {
    this->topPaddle_.controller(nullptr);

    this->bottomPaddle_.controller(std::shared_ptr<PaddleController>(
                                                    new MousePaddleController));
    this->bottomPaddle_.position(
//                                    ,--This is casted to a double here to
//                                   /   avoid narrowing conversion warnings. I
//                                  \/   don't really like it but, eh.
                    {0,static_cast<double>(Game::getInstance()->height()
                                        - this->bottomPaddle_.height())});

    //Set initial position of the ball. The center.
    math::vector ball_pos;
    ball_pos.x = center(0, Game::getInstance()->width(),
                        this->ball_.diameter());
    ball_pos.y = center(0, Game::getInstance()->height(),
                        this->ball_.diameter());
    this->ball_.position(ball_pos);

    //Set initial velocity of the ball.
    math::vector ball_vec;
    ball_vec.x = 0;
    ball_vec.y = -2;
    //Add the ball to our physics simulation.
    this->world_->addBall(&this->ball_, ball_vec);

    //Add the paddles to our physics simulation.
    this->world_->addPaddle(&this->topPaddle_);
    this->world_->addPaddle(&this->bottomPaddle_);

    //Configure the mouse.
    SDL_WM_GrabInput(SDL_GRAB_ON);
    SDL_ShowCursor(SDL_DISABLE);
  }
  SinglePlayerGameState::~SinglePlayerGameState()
  {
    delete this->world_;
  }

  void SinglePlayerGameState::update_private()
  {
    if(Game::getInstance()->events.keyHasBeenPressed())
    {
      Game::getInstance()->setGameState(std::shared_ptr<GameState>(
                                                            new MenuGameState));
    }

    this->topPaddle_.update();
    this->bottomPaddle_.update();
    this->world_->step();
  }
  void SinglePlayerGameState::render_private(SDL_Surface* surface) const
  {
    this->topPaddle_.render(surface);
    this->bottomPaddle_.render(surface);
    this->ball_.render(surface);
  }
};
