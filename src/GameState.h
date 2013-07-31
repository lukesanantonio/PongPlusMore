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
     */
    virtual void update() = 0;

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
