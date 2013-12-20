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

#include <iostream>
#include "SDL.h"
#include "common/crash.hpp"
#include "client/render_text.h"
#include "client/Button.h"
int main(int argc, char** argv)
{
  using pong::crash;
  if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
  {
    crash("Failed to initialize SDL2.");
  }
  SDL_Window* window = SDL_CreateWindow("Pong Plus More",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        1000, 1000,
                                        SDL_WINDOW_RESIZABLE);
  if(!window)
  {
    crash("Failed to create a SDL_Window*!");
  }
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
                                              SDL_RENDERER_ACCELERATED);
  if(!renderer)
  {
    crash("Failed to create an SDL_Renderer*!");
  }

  pong::MonoTextRenderer font_renderer("/home/luke/.sokoban/Sokoban.ttf");
  pong::Button button("Hello World!", {100, 100}, 100, 100, true, &font_renderer);

  pong::Button enable("Enable?", {0, 0}, 50, 50, true, &font_renderer);
  enable.executeOnClick([&button](){ button.enabled(!button.enabled()); });

  bool running = true;
  button.executeOnClick([&running]() { running = false; });

  SDL_SetRenderDrawColor(renderer, 0x00, 0xff, 0x00, 0xff);
  while(running)
  {
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
      button.handleEvent(event);
      enable.handleEvent(event);
    }

    SDL_RenderClear(renderer);
    button.render(renderer);
    enable.render(renderer);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
