#include "MenuGameState.h"
#include "Game.h"
#include "render_text.h"
#include "SinglePlayerGameState.h"
namespace pong
{
  MenuGameState::MenuGameState() noexcept : title_("Ultimate Pong", 110),
                                            singleplayer_("Singleplayer"),
                                            multiplayer_("Multiplayer"),
                                            quit_("Quit")
  {
    math::vector pos;
    pos.x = center(0, 1000, this->title_.getSurfaceWidth());
    pos.y = 40;
    this->title_.position(pos);

    std::size_t width = 550;
    this->singleplayer_.width(width);
    this->multiplayer_.width(width);
    this->quit_.width(width);

    std::size_t height = 75;
    this->singleplayer_.height(height);
    this->multiplayer_.height(height);
    this->quit_.height(height);

    std::size_t text_height = 50;
    this->singleplayer_.text_height(text_height);
    this->multiplayer_.text_height(text_height);
    this->quit_.text_height(text_height);

    //Since every button has the same width we can get the center of one, and
    //use it for all of them.
    int centerx = center(0, 1000, width);
    this->singleplayer_.position({centerx, 400});
    this->multiplayer_.position({centerx, 500});
    this->quit_.position({centerx, 600});

    this->singleplayer_.executeOnClick([]()
    {
      Game::getInstance()->setGameState(std::shared_ptr<GameState>(
                                                    new SinglePlayerGameState));
    });
    this->multiplayer_.executeOnClick([]()
    {
      Game::getInstance()->quitGame();
    });
    this->quit_.executeOnClick([]()
    {
      Game::getInstance()->quitGame();
    });
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
        case SDL_MOUSEBUTTONUP:
        {
          math::vector pos = {event.button.x, event.button.y};
          this->singleplayer_.checkClick(pos);
          this->multiplayer_.checkClick(pos);
          this->quit_.checkClick(pos);
          break;
        }
      }
    }
  }
  void MenuGameState::render(SDL_Surface* surface) const
  {
    this->title_.render(surface);

    this->singleplayer_.render(surface);
    this->multiplayer_.render(surface);
    this->quit_.render(surface);
  }
};
