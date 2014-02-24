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
 * \file Button.h
 * \brief Contains declarations for the Button class.
 */
#pragma once
namespace pong
{
  /*!
   * \brief Represents some object that can be rendered.
   */
  struct RenderableObject
  {
    virtual void render(SDL_Renderer* renderer) const noexcept = 0;
  };

  /*!
   * \brief An non-intrusive adapter to turn select objects into a
   * RenderableObject.
   */
  template <class object_type>
  struct RenderableObjectTemplate : RenderableObject
  {
    RenderableObjectTemplate(const object_type& obj) noexcept : obj_(obj) {}
    void render(SDL_Renderer* renderer) const noexcept override
    {
      obj_.render(renderer);
    }
  private:
    const object_type& obj_;
  };

  struct AnimationBase
  {
  public:
    virtual ~AnimationBase() noexcept {}

    /*!
     * \brief Make animation go. Specifically accounts for 1 ms of animation.
     */
    virtual void step() noexcept = 0;

    /*!
     * \brief Returns all objects that are part of the animation.
     */
    virtual std::vector<std::unique_ptr<RenderableObject> >
    objects() const noexcept = 0;
  };
}
