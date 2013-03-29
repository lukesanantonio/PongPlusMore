#ifndef ULTIMATE_PONG_MENU_GAME_STATE_H
#define ULTIMATE_PONG_MENU_GAME_STATE_H
#include "GameState.h"
namespace pong
{
  class MenuGameState : public GameState
  {
  public:
    MenuGameState() noexcept = default;
    ~MenuGameState() noexcept = default;
    MenuGameState(const MenuGameState&) = delete;
    MenuGameState(MenuGameState&&) = delete;
    MenuGameState& operator=(const MenuGameState&) = delete;
    MenuGameState& operator=(MenuGameState&&) = delete;
    
    virtual void update() override;
    virtual void render(SDL_Surface*) const override;
  };
};
#endif
