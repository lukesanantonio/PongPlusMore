/*!
 * \file Game.h
 * \brief Contains the declaration of the Game class.
 */
#ifndef ULTIMATE_PONG_GAME_H
#define ULTIMATE_PONG_GAME_H
#include <memory>
#include <SDL/SDL.h>
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
  private:
    /*!
     * \brief Default constructor using the default constructor behavior.
     */
    Game() = default;
    /*!
     * \brief Default destructor using the default destructor behavior.
     */
    ~Game() = default;
    
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
     * \brief The main surface created by SDL_SetVideoMode.
     */
    SDL_Surface* mainSurface_ = nullptr;
    
    /*!
     * \brief Initializes SDL for our needs.
     */
    void initializeSDL();
    
    /*!
     * \brief Uninitializes and undos everything which occurred in Game::init().
     */
    void uninitializeSDL();
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
};
#endif
