/*!
 * \file Game.h
 * \brief Contains the declaration of the Game class.
 */
#ifndef ULTIMATE_PONG_GAME_H
#define ULTIMATE_PONG_GAME_H
#include <memory>
#include <SDL/SDL.h>
#include "GameState.h"

/*!
 * \brief Namespace encapsulating all pong related anything...
 */
namespace pong
{
  /*!
   * \brief Singleton class encapsulated all global game crap.
   *
   * Manages a window, the game loop, some game state management, and more.
   */
  class Game
  {
  public:
    /*!
     * \brief Returns the one instance which will be created if necessary.
     * \return The singleton instance, created if necessary, for the one Game
     * object.
     */
    inline static Game* getInstance();
    
    /*!
     * \brief The main game loop.
     *
     * \param[in] argc The number of program arguments. Straight from the main
     * function.
     * \param[in] argv The argument vector. Straight from the main function.
     *
     * \returns An error code, 0 on success. Suitable for returning from the
     * main function.
     */
    int run(int argc, char* argv[]);
    
    /*!
     * \brief Sets the game state to the one passed in.
     *
     * \param gamestate The new game state to be used after this function has
     * returned.
     */
    void setGameState(std::shared_ptr<GameState> gamestate) noexcept;
    
    /*!
     * \brief Make sures that the game loop stops on the next iteration.
     */
    inline void quitGame();
  private:
    /*!
     * \brief Default constructor using the default constructor behavior.
     */
    Game() noexcept = default;
    /*!
     * \brief Default destructor using the default destructor behavior.
     */
    ~Game() noexcept = default;
    
    /*!
     * \brief Default copy constructor deleted to prevent use.
     */
    Game(const Game&) = delete;
    /*!
     * \brief Default move constructor deleted to prevent use.
     */
    Game(Game&&) = delete;
    
    /*!
     * \brief Default copy assignment operator deleted to prevent use.
     */
    Game& operator=(const Game&) = delete;
    /*!
     * \brief Default move assignment operator deleted to prevent use.
     */
    Game& operator=(Game&&) = delete;
    
    
    /*!
     * \brief The one instance of the Game class.
     */
    static std::shared_ptr<Game> instance_;
    
    /*!
     * \brief The main surface created by `SDL_SetVideoMode()`.
     */
    SDL_Surface* mainSurface_ = nullptr;
    
    /*!
     * \brief Initializes SDL for our needs.
     */
    void initializeSDL();
    
    /*!
     * \brief Uninitializes and undos everything which occurred in Game::init().
     */
    void uninitializeSDL() noexcept;
    
    /*!
     * \brief The current game state.
     *
     * GameState::render()'d and GameState::update()'d every iteration of the
     * game loop.
     *
     * \sa Game::setGameState(std::shared_ptr<GameState>)
     */
    std::shared_ptr<GameState> gameState_;
    
    /*!
     * \brief Whether or not the game state has been changed.
     *
     * If this is true, the game loop, will start over, or `'continue'` in C++
     * terms, without going further.
     * ie The game state is guaranteed to have been GameState::update()'d at
     * least once before it will have been GameState::render()'d.
     */
    bool gameStateChanged_ = false;
    
    /*!
     * \brief True when the game is currently running.
     *
     * \sa Game::quitGame()
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
  
  /*!
   * \post Game::running_ == false of the `this` object.
   */
  inline void Game::quitGame()
  {
    this->running_ = false;
  }
};
#endif
