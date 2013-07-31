/*!
 * \file RenderableEntity.h
 */
#pragma once
#include "SDL.h"
namespace pong
{
  /*!
   * \brief Real simple base class to allow generic handling of any entity which
   * can be rendered.
   *
   * The word 'Entity' here means, basically, a thing, anything, a GameState, an
   * object, anything that can be rendered!
   */
  struct RenderableEntity
  {
    virtual ~RenderableEntity() = default;

    /*!
     * Renders the object onto the surface...
     */
    virtual void render(SDL_Surface* surface) const = 0;
  };
};
