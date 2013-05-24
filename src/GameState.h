/*!
 * \file GameState.h
 * \brief File containing the ABC for all game states.
 */
#ifndef ULTIMATE_PONG_GAME_STATE_H
#define ULTIMATE_PONG_GAME_STATE_H
#include <SDL/SDL.h>
#include "EventSignals.h"
namespace pong
{
  /*!
   * \brief ABC which all game states should publicly inherit from.
   */
  class GameState
  {
  public:
    /*!
     * \brief Virtual destructor since we are going to delete using a
     * pointer-to-base.
     */
    virtual ~GameState() = default;

    /*!
     * \brief The interface to the real, private update function
     * (GameState::update_private()).
     */
    inline void update()
    {
      handleEvents(signals);
      this->update_private();
    }
    /*!
     * \brief The interface to the real, private render function
     * (GameState::render_private()).
     */
    inline void render(SDL_Surface* surface) const
    {
      this->render_private(surface);
    }
    EventSignals signals;
  private:
    /*!
     * \brief This function updates the internal state of the object.
     */
    virtual void update_private() = 0;

    /*!
     * \brief This function renders whatever it needs to render based off of
     * it's internal state set during GameState::update().
     *
     * It should not modify the object.
     */
    virtual void render_private(SDL_Surface* surface) const = 0;
  };
};
#endif
