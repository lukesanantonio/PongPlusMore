#include "MenuGameState.h"
#include "Game.h"
namespace pong
{
  void MenuGameState::update()
  {
    Game* game = Game::getInstance();
    
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
      switch(event.type)
      {
        case SDL_QUIT:
        case SDL_KEYDOWN:
        {
          game->quitGame();
        }
      }
    }
  }
  void MenuGameState::render(SDL_Surface* surface) const
  {
  }
};
