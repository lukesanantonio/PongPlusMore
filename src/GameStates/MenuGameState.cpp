/*!
 * \file MenuGameState.cpp
 * \brief File containing definitions for the MenuGameState
 */
#include "MenuGameState.h"
#include "Game.h"
#include "render_text.h"
#include "SinglePlayerGameState.h"
namespace pong
{
  MenuGameState::MenuGameState() noexcept : title_("Ultimate Pong", 110),
                                            singleplayer_("Singleplayer"),
                                            multiplayer_("Multiplayer"),
                                            options_("Options"),
                                            help_("Help"),
                                            quit_("Quit")
  {
    math::vector pos;
    pos.x = center(0, 1000, this->title_.getSurfaceWidth());
    pos.y = 40;
    this->title_.position(pos);

    std::size_t width = 550;
    std::size_t height = 75;
    std::size_t text_height = 50;
    pos.x = center(0, 1000, width);

    pos.y = 300;
    this->singleplayer_.position(pos);
    this->singleplayer_.width(width);
    this->singleplayer_.height(height);
    //this->singleplayer_.enabled(false);

    pos.y += 100;
    this->multiplayer_.position(pos);
    this->multiplayer_.width(width);
    this->multiplayer_.height(height);
    this->multiplayer_.enabled(false);

    pos.y += 100;
    this->options_.position(pos);
    this->options_.width(width);
    this->options_.height(height);
    this->options_.enabled(false);

    pos.y += 100;
    this->help_.position(pos);
    this->help_.width(width);
    this->help_.height(height);
    this->help_.enabled(false);

    pos.y += 100;
    this->quit_.position(pos);
    this->quit_.width(width);
    this->quit_.height(height);

    this->singleplayer_.executeOnClick([]()
    {
      Game::getInstance()->setGameState(std::shared_ptr<GameState>(
                                                    new SinglePlayerGameState));
    });
    this->quit_.executeOnClick([this]()
    {
      Game::getInstance()->quitGame();
    });

    //Configure the mouse
    SDL_WM_GrabInput(SDL_GRAB_OFF);
    SDL_ShowCursor(SDL_ENABLE);
  }
  void MenuGameState::update_private()
  {
    //Check for Quit event.
    Game* game = Game::getInstance();
    if(game->events.windowHasBeenClosed() or
       game->events.keyHasBeenPressed())
    {
      Game::getInstance()->quitGame();
    }

    //Check for collisions.
    singleplayer_.update();
    multiplayer_.update();
    options_.update();
    help_.update();
    quit_.update();
  }
  void MenuGameState::render_private(SDL_Surface* surface) const
  {
    this->title_.render(surface);

    this->singleplayer_.render(surface);
    this->multiplayer_.render(surface);
    this->options_.render(surface);
    this->help_.render(surface);
    this->quit_.render(surface);
  }
};
