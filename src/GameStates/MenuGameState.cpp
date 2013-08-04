/*!
 * \file MenuGameState.cpp
 * \brief File containing definitions for the MenuGameState
 */
#include "MenuGameState.h"
#include "render_text.h"
//#include "SinglePlayerGameState.h"
namespace pong
{
  MenuGameState::MenuGameState(Game* game) noexcept :
                                           title_("Ultimate Pong", 110),
                                           singleplayer_("Singleplayer"),
                                           multiplayer_("Multiplayer"),
                                           options_("Options"),
                                           help_("Help"),
                                           quit_("Quit"),
                                           game_(game)
  {
    this->title_.font_renderer(game->font_renderer());

    math::vector pos;
    pos.x = center(0, 1000, this->title_.getSurfaceWidth());
    pos.y = 40;
    this->title_.position(pos);

    std::size_t width = 550;
    std::size_t height = 75;
    pos.x = center(0, 1000, width);

    pos.y = 300;
    this->singleplayer_.position(pos);
    this->singleplayer_.width(width);
    this->singleplayer_.height(height);
    this->singleplayer_.enabled(false);
    this->singleplayer_.font_renderer(game->font_renderer());

    pos.y += 100;
    this->multiplayer_.position(pos);
    this->multiplayer_.width(width);
    this->multiplayer_.height(height);
    this->multiplayer_.enabled(false);
    this->multiplayer_.font_renderer(game->font_renderer());

    pos.y += 100;
    this->options_.position(pos);
    this->options_.width(width);
    this->options_.height(height);
    this->options_.enabled(false);
    this->options_.font_renderer(game->font_renderer());

    pos.y += 100;
    this->help_.position(pos);
    this->help_.width(width);
    this->help_.height(height);
    this->help_.enabled(false);
    this->help_.font_renderer(game->font_renderer());

    pos.y += 100;
    this->quit_.position(pos);
    this->quit_.width(width);
    this->quit_.height(height);
    this->quit_.font_renderer(game->font_renderer());

    this->addRenderableEntity(&this->title_);
    this->addRenderableEntity(&this->singleplayer_);
    this->addRenderableEntity(&this->multiplayer_);
    this->addRenderableEntity(&this->options_);
    this->addRenderableEntity(&this->help_);
    this->addRenderableEntity(&this->quit_);

#if 0
    this->singleplayer_.executeOnClick([]()
    {
      Game::getInstance()->setGameState(std::shared_ptr<GameState>(
                                                    new SinglePlayerGameState));
    });
#endif

    this->quit_.executeOnClick([this, game]()
    {
      //If the button is getting updated, we can assume we are on top of the
      //stack! TODO: This seems fishy, even though it is valid! Do something
      //about it!
      game->popGameState();
    });

    //Configure the mouse
    SDL_WM_GrabInput(SDL_GRAB_OFF);
    SDL_ShowCursor(SDL_ENABLE);
  }
  PostUpdateAction MenuGameState::update()
  {
    return PostUpdateAction::Render;
  }
  void MenuGameState::handleEvent(const SDL_Event& event)
  {
    if(event.type == SDL_KEYUP)
    {
      this->game_->popGameState();
    }

    this->singleplayer_.handleEvent(event);
    this->multiplayer_.handleEvent(event);
    this->options_.handleEvent(event);
    this->help_.handleEvent(event);
    this->quit_.handleEvent(event);
  }
};
