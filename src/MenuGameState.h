/*!
 * \file MenuGameState.h
 * \brief Declarations for the menu game state.
 */
#ifndef ULTIMATE_PONG_MENU_GAME_STATE_H
#define ULTIMATE_PONG_MENU_GAME_STATE_H
#include "GameState.h"
namespace pong
{
  /*!
   * \brief GameState handling the initial menu when the user first loads the
   * program.
   */
  class MenuGameState : public GameState
  {
  public:
    /*!
     * \brief Default constructor set to the default behavior.
     */
    MenuGameState() noexcept = default;
    /*!
     * \brief Default destructor set to the default behavior.
     */
    ~MenuGameState() noexcept = default;
    
    /*!
     * \brief Copy constructor, deleted.
     */
    MenuGameState(const MenuGameState&) = delete;
    /*!
     * \brief Move constructor, deleted.
     */
    MenuGameState(MenuGameState&&) = delete;
    
    /*!
     * \brief Copy assignment operator, deleted.
     */
    MenuGameState& operator=(const MenuGameState&) = delete;
    /*!
     * \brief Move assignment operator, deleted.
     */
    MenuGameState& operator=(MenuGameState&&) = delete;
    
    virtual void update();
    virtual void render(SDL_Surface*) const noexcept;
  };
};
#endif
