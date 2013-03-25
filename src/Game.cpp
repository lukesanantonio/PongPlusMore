/*!
 * \file Game.cpp
 * \brief Contains the definitions of the Game class.
 *
 * This is where the main function and game loop are implemented.
 */
#include "Game.h"
#include <stdexcept>
#include "scoped_init.hpp"
namespace pong
{
  std::shared_ptr<Game> Game::instance_ = nullptr;
  
  int Game::run(int argc, char* argv[])
  {
    scoped_init<std::function<void ()>, std::function<void ()> >
                  SDLinit(std::bind(&Game::initializeSDL,   this),
                          std::bind(&Game::uninitializeSDL, this));
    return 0;
  }
  
  /*!
   * \post Set's Game::mainSurface_ to the return of SDL_SetVideoMode.
   */
  void Game::initializeSDL()
  {
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
      throw std::runtime_error("Failed to initialize SDL");
    }
    //Satisfy the post condition.
    this->mainSurface_ = SDL_SetVideoMode(1000, 1000, 32, SDL_HWSURFACE |
                                                          SDL_DOUBLEBUF);
    if(!this->mainSurface_)
    {
      throw std::runtime_error("Failed to set SDL video mode.");
    }
  }
  
  /*!
   * \post Set's Game::mainSurface_ to `nullptr` after SDL_FreeSurface()'ing it.
   */
  void Game::uninitializeSDL() noexcept
  {
    //Free the surface.
    SDL_FreeSurface(this->mainSurface_);
    
    //Satisfy the post condition.
    this->mainSurface_ = nullptr;
    
    //Quit sdl.
    SDL_Quit();
  }
};

int main(int argc, char* argv[])
{
  pong::Game* game = pong::Game::getInstance();
  return game->run(argc, argv);
}
