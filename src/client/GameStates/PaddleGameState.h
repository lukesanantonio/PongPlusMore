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
#pragma once
#include "../Game.h"
#include "../GameState.h"
#include "server/LocalServer.h"
#include "common/serialize.h"
#include <iostream>
namespace pong
{
  enum class PaddleOrientation
  {
    Vertical,
    Horizontal
  };

  struct PaddleInput
  {
    PaddleInput(id_type id, const ObjectManager& objs, PaddleOrientation o)
                     : id_(id), objs_(objs), o_(o) {}
    virtual double get_position() const noexcept = 0;

    PaddleInput(const PaddleInput&) = delete;
    PaddleInput(PaddleInput&&) = delete;
    PaddleInput& operator=(const PaddleInput&) = delete;
    PaddleInput& operator=(PaddleInput&&) = delete;
  protected:
    const id_type id_;
    const ObjectManager& objs_;
    const PaddleOrientation o_;
  };

  struct MouseInput : public PaddleInput
  {
    using PaddleInput::PaddleInput;
    inline double get_position() const noexcept override;
  };
  double MouseInput::get_position() const noexcept
  {
    if(this->o_ == PaddleOrientation::Vertical)
    {
      int x;
      SDL_GetMouseState(&x, NULL);
      return x;
    }
    else
    {
      int y;
      SDL_GetMouseState(NULL, &y);
      return y;
    }
  }

  struct PaddleGameState : public GameState
  {
  public:
    PaddleGameState(Game& g, Volume v, PaddleOrientation o);
    virtual void handleEvent(const SDL_Event& event) override;
    virtual void update() override;
    virtual void render(SDL_Renderer*) const override;
  private:
    Game& g_;
    LocalServer server_;
    id_type top_ = 0;
    id_type bottom_ = 0;
    id_type ball_ = 0;
    bool render_quadtree_ = false;
    bool render_constraints_ = false;

    int top_score_ = 0;
    int bottom_score_ = 0;

    PaddleOrientation o_;

    // Wrapped in a unique_ptr for lazy initialization.
    std::unique_ptr<MouseInput> top_input_;
    std::unique_ptr<MouseInput> bot_input_;
  };
}
