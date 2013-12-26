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
#include "render.h"
namespace pong
{
  template <typename int_type>
  static void render_rect(SDL_Renderer* renderer,
                          int_type x, int_type y,
                          int_type w, int_type h)
  {
    SDL_Rect rect{x, y, w, h};
    SDL_RenderFillRect(renderer, &rect);
  }
  inline void render(SDL_Renderer* renderer, const Paddle& paddle)
  {
    render_rect(renderer, paddle.pos.x, paddle.pos.y,
                          paddle.width, paddle.height);
  }
  inline void render(SDL_Renderer* renderer, const Ball& ball)
  {
    render_rect(renderer, ball.pos.x, ball.pos.y,
                          ball.diameter, ball.diameter);
  }
}
