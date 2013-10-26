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
/*!
 * \file Game.h
 * \brief All global, game-wide functions and classes are (should be) here.
 */
#pragma once
#include <memory>
#include <vector>
#include <stack>
#include "SDL.h"
#include "GameState.h"
#include "render_text.h"
#include "GameSettings.h"

int main(int argc, char* argv[]);

/*!
 * \brief Contains any code written specifically for ultimate pong.
 */
namespace pong
{
  /*!
   * \brief Main logic of the game featuring initialization!
   *
   * The Game class' constructor is private as is the run function, since, the
   * users of the game class are worried about managing how the game progresses
   * (GameStates, etc.), how it does all this is an implementation detail...
   */
  class Game
  {
  public:
    /*!
     * \brief The main stack of GameStates.
     *
     * The fact that they are shared pointers are an implementation detail...
     */
    using GameStateStack =std::stack<std::shared_ptr<GameState>,
                                     std::vector<std::shared_ptr<GameState> > >;
  public:
    void pushGameState(std::unique_ptr<GameState>);
    void popGameState();

    inline void quit() noexcept;

    inline FontRenderer* font_renderer() const noexcept;

    inline int width() const noexcept;
    inline int height() const noexcept;
  private:
    Game() = default;
    ~Game() = default;
    Game(const Game&) = delete;
    Game(Game&&) = delete;
    Game& operator=(const Game&) = delete;
    Game& operator=(Game&&) = delete;

    bool initializeSDL(unsigned int width, unsigned int height);
    void uninitializeSDL();

    SDL_Surface* main_surface_ = nullptr;

    bool running_ = true;

    /*!
     * \brief The font renderer implementation, obtained from the config file.
     *
     * It should be used and passed along to member objects by GameStates!
     *
     * \note It can be cached, but not between GameStates, meaning it should be
     * cached again every time a new GameState is constructed!
     */
    FontRenderer* font_renderer_ = nullptr;

    int run(int argc, char* argv[]);

    friend int ::main(int argc, char* argv[]);

    GameStateStack game_state_stack_;
  };

  inline void Game::quit() noexcept
  {
    this->running_ = false;
  }

  inline FontRenderer* Game::font_renderer() const noexcept
  {
    return this->font_renderer_;
  }

  inline int Game::width() const noexcept
  {
    return this->main_surface_->w;
  }
  inline int Game::height() const noexcept
  {
    return this->main_surface_->h;
  }
};
