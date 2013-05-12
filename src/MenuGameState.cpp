#include "MenuGameState.h"
#include "Game.h"
#include "render_text.h"
namespace pong
{
  MenuGameState::MenuGameState() noexcept : title_("Ultimate Pong", 110)
  {
    math::vector pos;
    pos.x = center(0, 1000, this->title_.getSurfaceWidth());
    pos.y = 40;
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
          break;
        }
      }
    }
  }
  void MenuGameState::render(SDL_Surface* surface) const
  {
    this->title_.render(surface);
  }
};
