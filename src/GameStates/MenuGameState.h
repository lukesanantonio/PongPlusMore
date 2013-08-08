/*!
 * \file MenuGameState.h
 * \brief File containing declarations of the MenuGameState.
 */
#pragma once
#include "GameState.h"
#include "Label.h"
#include "Button.h"
#include "Game.h"
namespace pong
{
  /*!
   * \brief First game state shown, the menu.
   */
  class MenuGameState : public GameState
  {
  public:
    MenuGameState(Game& game) noexcept;
    ~MenuGameState() noexcept = default;
    MenuGameState(const MenuGameState&) = delete;
    MenuGameState(MenuGameState&&) = delete;
    MenuGameState& operator=(const MenuGameState&) = delete;
    MenuGameState& operator=(MenuGameState&&) = delete;

    virtual void update() override {}
    virtual void handleEvent(const SDL_Event& event) override;
  private:
    Label title_;

    Button singleplayer_;
    Button multiplayer_;
    Button options_;
    Button help_;
    Button quit_;

    Game& game_;
  };
};
