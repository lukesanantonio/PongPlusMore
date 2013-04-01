/*!
 * \file GameState.h
 * \brief File containing the ABC for all game states.
 */
#ifndef ULTIMATE_PONG_GAME_STATE_H
#define ULTIMATE_PONG_GAME_STATE_H
#include <SDL/SDL.h>
namespace pong
{
  /*!
   * \brief ABC which all game states should publicly inherit from.
   */
  class GameState
  {
  public:
    /*!
     * \brief This function updates the internal state of the object.
     */
    virtual void update() = 0;

    /*!
     * \brief This function renders whatever it needs to render based off of
     * it's internal state set during GameState::update().
     *
     * It should not modify the object.
     */
    virtual void render(SDL_Surface* surface) const = 0;
  };
};
#endif
