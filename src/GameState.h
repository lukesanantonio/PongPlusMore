#ifndef ULTIMATE_PONG_GAME_STATE_H
#define ULTIMATE_PONG_GAME_STATE_H
#include <SDL/SDL.h>
namespace pong
{
  class GameState
  {
  public:
    virtual void update() = 0;
    virtual void render(SDL_Surface* surface) const noexcept = 0;
  };
};
#endif
