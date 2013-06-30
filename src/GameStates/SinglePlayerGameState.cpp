/*!
 * \file SinglePlayerGameState.cpp
 * \brief Contains definitions for the SinglePlayerGameState GameState.
 */
#include "SinglePlayerGameState.h"
#include "Game.h"
#include "PaddleControllers/MousePaddleController.h"
#include "GameStates/MenuGameState.h"
namespace pong
{
  SinglePlayerGameState::SinglePlayerGameState()
  {
    this->topPaddle_.controller(nullptr);

    this->bottomPaddle_.controller(std::shared_ptr<PaddleController>(
                                                    new MousePaddleController));
    this->bottomPaddle_.position(
                    {0,static_cast<int>(Game::getInstance()->height()
                                        - this->bottomPaddle_.height())});

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
    this->topPaddle_.update();
    this->bottomPaddle_.update();
  }
  void SinglePlayerGameState::render_private(SDL_Surface* surface) const
  {
    this->topPaddle_.render(surface);
    this->bottomPaddle_.render(surface);
  }
};
