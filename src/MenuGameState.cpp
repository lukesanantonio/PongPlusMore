#include "MenuGameState.h"
#include "Game.h"
namespace pong
{
  MenuGameState::MenuGameState() noexcept : title_("Ultimate Pong", 100)
  {
    math::vector pos;
    pos.x = (1000 / 2) - (this->title_.getSurfaceWidth() / 2);
    pos.y = 20;
    this->title_.position(pos);
  }
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
    this->title_.render(surface);
  }
};
