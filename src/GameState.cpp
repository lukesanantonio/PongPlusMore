/*
 * PpM - Pong Plus More - A pong clone full of surprises written with C++11.
 * Copyright (C) 2013  Luke San Antonio
 *
 * You can contact me (Luke San Antonio) at lukesanantonio@gmail.com!
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "GameState.h"
#include <algorithm>
namespace pong
{
  /*!
   * \brief Renders all entities in GameState::render_queue_ which are valid
   * pointers.
   *
   * Since this is a GameState render function the surface is cleared when it
   * needs to be, though don't count on it! In other words, clearing and
   * flipping the surface should *only* occur here, in the game state render
   * function!
   *
   * \warning The passed in surface needs to be the main surface generated with
   * SDL_SetVideoMode() otherwise... You're on your own! (ie Undefined Behavior)
   */
  void GameState::render(SDL_Surface* surface) const
  {
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0x00, 0x00, 0x00));

    //Render!
    for(const RenderableEntity* entity : this->render_queue_)
    {
      if(!entity) continue;
      entity->render(surface);
    }

    SDL_Flip(surface);
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
