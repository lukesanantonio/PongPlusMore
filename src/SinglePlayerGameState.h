#ifndef ULTIMATE_PONG_SINGLE_PLAYER_GAME_STATE_H
#define ULTIMATE_PONG_SINGLE_PLAYER_GAME_STATE_H
#include "GameState.h"
namespace pong
{
  class SinglePlayerGameState : public GameState
  {
  public:
    virtual void update();
    virtual void render(SDL_Surface*) const;
  };
};
#endif