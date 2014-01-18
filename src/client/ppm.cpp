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
#include "common/Timer.hpp"
#include "SDL.h"
#include "common/crash.hpp"
#include "client/Game.h"
#include "client/GameStates/MenuGameState.h"

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

  pong::Game game;
  game.width = 1000;
  game.height = 1000;
  game.font_renderer.reset(
                  new pong::MonoTextRenderer("/home/luke/.fonts/Railway.ttf"));
  game.game_state.reset(new pong::MenuGameState(game));

  pong::Timer<> update;
  while(!game.exiting)
  {
    if(update.hasBeen(std::chrono::milliseconds(5)))
    {
      // Make a link to the game state so it doesn't all of a sudden become a
      // nullptr.
      std::shared_ptr<pong::GameState> game_state = game.game_state;

      SDL_Event event;
      while(SDL_PollEvent(&event))
      {
        game_state->handleEvent(event);
      }

      game_state->update();
      update.reset();

      // If the game state is different from our cache... Don't render.
      if(game_state != game.game_state) continue;

      SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
      SDL_RenderClear(renderer);

      game_state->render(renderer);

      SDL_RenderPresent(renderer);
    }
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
