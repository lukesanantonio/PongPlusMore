#include "MenuGameState.h"
#include "Game.h"
#include "render_text.h"
namespace pong
{
  MenuGameState::MenuGameState() noexcept : title_("Ultimate Pong", 110),
                                            singleplayer_("Singleplayer"),
                                            multiplayer_("Multiplayer"),
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

    pos.y = 375;
    this->singleplayer_.position(pos);
    this->singleplayer_.width(width);
    this->singleplayer_.height(height);
    this->singleplayer_.text_height(text_height);

    pos.y += 100;
    this->multiplayer_.position(pos);
    this->multiplayer_.width(width);
    this->multiplayer_.height(height);
    this->multiplayer_.text_height(text_height);

    pos.y += 100;
    this->help_.position(pos);
    this->help_.width(width);
    this->help_.height(height);
    this->help_.text_height(text_height);

    pos.y += 100;
    this->quit_.position(pos);
    this->quit_.width(width);
    this->quit_.height(height);
    this->quit_.text_height(text_height);

    this->singleplayer_.addHandler(this->signals);
    this->multiplayer_.addHandler(this->signals);
    this->help_.addHandler(this->signals);
    this->quit_.addHandler(this->signals);

    this->quit_.executeOnClick([this]()
    {
      this->signals.on_quit();
    });

    this->signals.on_quit.connect([]()
    {
      Game::getInstance()->quitGame();
    });

  }
  void MenuGameState::render_private(SDL_Surface* surface) const
  {
    this->title_.render(surface);

    this->singleplayer_.render(surface);
    this->multiplayer_.render(surface);
    this->help_.render(surface);
    this->quit_.render(surface);
  }
};
