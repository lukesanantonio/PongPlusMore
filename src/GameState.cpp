#include "GameState.h"
#include <algorithm>
namespace pong
{
  /*!
   * \brief Renders all entities in GameState::render_queue_ which are valid
   * pointers.
   */
  void GameState::render(SDL_Surface* surface) const
  {
    //Render!
    for(const RenderableEntity* entity : this->render_queue_)
    {
      if(!entity) continue;
      entity->render(surface);
    }
  }

  /*!
   * \brief Adds a renderable entity to the scene graph...
   *
   * It will be rendered, when it needs to be rendered! That would be when
   * GameState::render is called...
   */
  void GameState::addRenderableEntity(const RenderableEntity* entity)
  {
    //If the entity is a nullptr, just return - we don't need it!
    if(!entity) return;

    //If its already in the queue, just return - we don't need it!
    if(std::find(this->render_queue_.begin(),
                 this->render_queue_.end(),
                 entity) != this->render_queue_.end()) return;

    this->render_queue_.push_back(entity);
  };
  void GameState::removeRenderableEntity(const RenderableEntity* entity)
  {
    this->render_queue_.erase(std::remove(this->render_queue_.begin(),
                                          this->render_queue_.end(),
                                          entity), this->render_queue_.end());
  }
};
