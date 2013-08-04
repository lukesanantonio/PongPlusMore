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

    bool initializeSDL();
    void uninitializeSDL();

    SDL_Surface* main_surface_ = nullptr;

    bool running_ = true;

    /*!
     * \brief The default font renderer, global to all who have a pointer to
     * the Game class.
     *
     * It need not be used at all!
     */
    std::unique_ptr<FontRenderer> font_renderer_ = nullptr;

    int run(int argc, char* argv[]);

    friend int ::main(int argc, char* argv[]);

    GameStateStack game_state_stack_;
  };

  inline FontRenderer* Game::font_renderer() const noexcept
  {
    return this->font_renderer_.get();
  }
};
