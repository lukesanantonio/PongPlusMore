/*!
 * \file MenuGameState.h
 * \brief File containing declarations of the MenuGameState.
 */
#ifndef ULTIMATE_PONG_MENU_GAME_STATE_H
#define ULTIMATE_PONG_MENU_GAME_STATE_H
#include "GameState.h"
#include "Label.h"
#include "Button.h"
namespace pong
{
  /*!
   * \brief First game state shown, the menu.
   */
  class MenuGameState : public GameState
  {
  public:
    MenuGameState() noexcept;
    ~MenuGameState() noexcept = default;
    MenuGameState(const MenuGameState&) = delete;
    MenuGameState(MenuGameState&&) = delete;
    MenuGameState& operator=(const MenuGameState&) = delete;
    MenuGameState& operator=(MenuGameState&&) = delete;

  private:
    virtual void update_private() override;
    virtual void render_private(SDL_Surface*) const override;

  private:
    Label title_;
    Button singleplayer_;
    Button multiplayer_;
    Button options_;
    Button help_;
    Button quit_;
  };
};
#endif
