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

    inline FontRenderer* font_renderer() const noexcept;
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

  inline FontRenderer* Game::font_renderer() const noexcept
  {
    return this->font_renderer_;
  }
};
