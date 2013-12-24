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
 *
 * \file abstract.h
 * \brief The classes that generalize objects and the like.
 *
 * The classes here will help to generalize the majority of things hopefully
 * allowing the code to make reasonable assumptions and therefore reduce
 * repeating code and effort.
 */
#pragma once
namespace pong
{
  enum class Setting
  {
    FontRenderer,
    Size
  };
  /*!
   * \brief A generic interface to settings management.
   *
   * One thing I really want to avoid is having to restart the program to
   * change settings, it's horrible and stupid. The only way to do this is to
   * be able to notify things (GameStates, Objects, etc) about the change so
   * that they can mainly update a cache. For instance when the font renderer
   * changes to an antialiased setting the fonts will not be regenerated,
   * that's why we need a form of interrupt. Hopefully a system like this will
   * allow the change to be propagated further to the children of the original
   * object. it. This seems like kind of a java-like approach to the problem,
   * and this seems weird to me, but what is there to do. At least we have
   * multiple inheritance. *Ha*.
   */
  struct SettingsAcceptor
  {
    /*!
     * Called after a setting has been changed. This is just the little
     * coercion needed to invalidate the caches.
     */
    virtual void confChange(Setting changed_setting) = 0;
  };

  struct RenderableEntity
  {
    /*!
     * \brief Renders the object using the passed in renderer.
     */
    virtual void render(SDL_Renderer*) const = 0;
  };

  struct EventHandler
  {
    virtual void handleEvent(SDL_Event& event) = 0;
  };

  struct UpdatableEntity
  {
    virtual void update() = 0;
  };
};
