/*!
 * \file GameState.h
 * \brief GameState ABC.
 */
#pragma once
#include <vector>
#include "RenderableEntity.h"
#include "EventHandler.h"
namespace pong
{
  /*!
   * \brief A signal to tell the Game what to do next!
   */
  enum class PostUpdateAction
  {
    DoNothing, //!< No rendering is necessary, don't clear the screen though!
    Render, //!< Render, we still have crap to do!
    ExitGameState, //!< Exit the GameState, ie Game::popGameState().
    ExitGame //!< Exit the game!
  };

  class GameState : public EventHandler, public RenderableEntity
  {
  public:
    /*!
     * \brief Notifies the GameState of an event.
     *
     * A more specific kind of update function.
     */
    virtual void handleEvent(const SDL_Event& event) override = 0;

    /*!
     * \brief Allows the GameState to update periodically and not necessarily
     * event-based.
     *
     * \returns A signal to the Game telling it what to do next.
     * \sa PostUpdateAction
     */
    virtual PostUpdateAction update() = 0;

    virtual void render(SDL_Surface*) const final override;

    void addRenderableEntity(const RenderableEntity* entity);
    void removeRenderableEntity(const RenderableEntity* entity);
  private:
    /*!
     * \brief The list of objects to render during GameState::render.
     */
    std::vector<const RenderableEntity*> render_queue_;
  };
};
