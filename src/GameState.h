/*!
 * \file GameState.h
 * \brief Contains declaration of the ABC for game states.
 */
#ifndef ULTIMATE_PONG_GAME_STATE_H
#define ULTIMATE_PONG_GAME_STATE_H
#include <SDL/SDL.h>
namespace pong
{
  /*!
   * \brief Abstract base class for any game state.
   */
  class GameState
  {
  public:
    /*!
     * \brief Should update internal class state so the renderer can render
     * that state.
     *
     * \note This is where events should be handled. Since they are not handled
     * anywhere else.
     */
    virtual void update() = 0;
    
    /*! 
     * \brief Renders onto the passed in surface, without changing internal
     * class state.
     *
     * \param surface This is assumed to be the main surface, everything
     * should be rendered here.
     *
     * \note Changing the internal class state should be left for
     * GameState::update().
     */
    virtual void render(SDL_Surface* surface) const noexcept = 0;
  };
};
#endif
