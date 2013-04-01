/*!
 * \file Game.h
 * \brief All global, game-wide functions and classes are (should be) here.
 */
#ifndef ULTIMATE_PONG_GAME_H
#define ULTIMATE_PONG_GAME_H
#include <memory>
#include <SDL/SDL.h>
#include "GameState.h"
/*!
 * \brief Contains any code written specifically for ultimate pong.
 */
namespace pong
{
  /*!
   * \brief Singleton which provides global procedures such as switching game
   * states and quitting the game.
   */
  class Game
  {
  public:
    /*!
     * \brief Returns a pointer to the one, global instance of the Game.
     *
     * If one has not yet been initialized, it will be initialized on the spot.
     *
     * \returns Game::instance_ before making sure it points to somewhere.
     *
     * \note If you delete the pointer returned, you can screw yourself -
     * *Luke*.
     */
    inline static Game* getInstance();

    /*!
     * \brief Changes the current game state.
     *
     * \param game_state A shared_ptr to a new game state. The new game state is
     * guaranteed to be GameState::update()'d exactly once, before being
     * GameState::render()'d, you can count on that.
     *
     * \note If you pass in a nullptr it's actually handled, nothing happens.
     * No game state change, etc. That means you don't have to screw yourself -
     * *Luke*.
     */
    inline void setGameState(std::shared_ptr<GameState> game_state) noexcept;

    /*!
     * \brief Sets up the game to exit on the next iteration of the main loop.
     *
     * That means as soon as possible.
     *
     * \post Game::running_ == false
     *
     * \note This is currently irreversible, if it is really necessary to undo,
     * you can screw yourself - *Luke*.
     */
    inline void quitGame() noexcept;

    /*!
     * \brief Starts (and finishes) the *main game loop*.
     *
     * This function is basically used for the real main function to call as
     * soon as possible. Also, instead of delegating an options parser to the
     * main function, we just get the options forwarded right to our door. It's
     * easier that way.
     *
     * \param[in] argc The amount of arguments in `argv`.
     * \param[in] argv The argument vector. An array of null terminated strings.
     * They are guaranteed not to be modified by our function.
     */
    int run(int argc, char* argv[]);
  private:
    /*!
     * \brief Trivial constructor.
     */
    Game() noexcept = default;
    /*!
     * \brief Trivial destructor.
     */
    ~Game() noexcept = default;

    /*!
     * \brief Deleted copy constructor.
     *
     * Deleted because: our Game class is a singleton, there should only be
     * one instance.
     */
    Game(const Game&) noexcept = delete;
    /*!
     * \brief Deleted move constructor.
     *
     * Deleted because: our Game class is a singleton, to move ownership out of
     * the class to some client code is equivalent to screwing yourself -
     * *Luke*.
     */
    Game(Game&&) noexcept = delete;

    /*!
     * \brief Deleted copy assignment operator.
     *
     * Deleted because: our Game class is self sufficient in the sense that
     * it manages itself, to copy that would screw up the whole singleton
     * pattern, you might as well screw yourself - *Luke*.
     */
    Game& operator=(const Game&) noexcept = delete;

    /*!
     * \brief Deleted move assignment operator.
     *
     * Deleted because: our Game class is a singleton, to move it is to screw
     * yourself - *Luke*. That's why we disallow it.
     */
    Game& operator=(Game&&) noexcept = delete;

    /*!
     * \brief The one Game instance.
     *
     * Allocated on the heap of course.\n
     * Managed smartly by a smart pointer of course.\n
     * Static, because it's a singleton of course.\n
     * Suffixed with an underscore because it's private of course.\n
     * Finished off with a semicolon because that's how C++ works of course. -
     * *Luke*.
     */
    static std::shared_ptr<Game> instance_;

    /*!
     * \brief initializes SDL and creates a window.
     *
     * \post Sets Game::main_surface_ to an implementation defined call to
     * `SDL_SetVideoMode()`.
     */
    void initializeSDL();

    /*!
     * \brief Undos everything done in Game::initializeSDL().
     *
     * \post Sets Game::main_surface_ = nullptr after `SDL_FreeSurface()`'ing
     * it.
     */
    void uninitializeSDL() noexcept;

    /*!
     * \brief The main surface from SDL. Created by 'setting the video mode'.
     *
     * \sa Game::initializeSDL()
     */
    SDL_Surface* main_surface_ = nullptr;

    /*!
     * \brief The current game state.
     */
    std::shared_ptr<GameState> game_state_ = nullptr;
    /*!
     * \brief Whether or not the game state has been changed since *erm* it's
     * been handled. This is changed to true in Game::setGameState() to tell
     * the main loop to start again to give the game state an update before
     * rendering.
     */
    bool game_state_changed_ = false;

    /*!
     * \brief Set to false when the game is scheduled to end.
     *
     * \sa Game::quitGame();
     */
    bool running_ = true;
  };

  inline Game* Game::getInstance()
  {
    if(!Game::instance_)
    {
      Game::instance_ = std::shared_ptr<Game>(new Game,
      [](Game* ptr)
      {
        delete ptr;
      });
    }
    return Game::instance_.get();
  }
  inline void Game::quitGame() noexcept
  {
    this->running_ = false;
  }
  inline void Game::setGameState(std::shared_ptr<GameState> game_state) noexcept
  {
    if(!game_state)
    {
      return;
    }
    this->game_state_ = game_state;
    this->game_state_changed_ = true;
  }
};
#endif
