#include "SinglePlayerGameState.h"
#include "Game.h"
namespace pong
{
  SinglePlayerGameState::SinglePlayerGameState()
  {
    this->signals.on_quit.connect([]()
    {
      Game::getInstance()->quitGame();
    });
  }
};
