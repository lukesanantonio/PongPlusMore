#include "Game.h"
#include <stdexcept>
#include "scoped_init.hpp"
#include "MenuGameState.h"
namespace pong
{
  std::shared_ptr<Game> Game::instance_ = nullptr;
  
  int Game::run(int argc, char* argv[])
  {
    scoped_init<std::function<void ()>, std::function<void ()> >
                  SDLinit(std::bind(&Game::initializeSDL,   this),
                          std::bind(&Game::uninitializeSDL, this));
    this->gameState_ = std::shared_ptr<GameState>(new MenuGameState);
    while(this->running_)
    {
      this->gameState_->update();
      if(this->gameStateChanged_)
      {
        //the game state changed?
        //alright, continue... but don't forget to prevent this loop from
        //repeating the next iteration of the game loop.
        this->gameStateChanged_ = false;
        continue;
      }
      this->gameState_->render(this->mainSurface_);
      SDL_Flip(this->mainSurface_);
    }
    return 0;
  }
  
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
      SDL_Quit();
      throw std::runtime_error("Failed to set SDL video mode.");
    }
  }
  
  void Game::uninitializeSDL() noexcept
  {
    //Free the surface.
    SDL_FreeSurface(this->mainSurface_);
    
    //Satisfy the post condition.
    this->mainSurface_ = nullptr;
    
    //Quit sdl.
    SDL_Quit();
  }
  
  void Game::setGameState(std::shared_ptr<GameState> gamestate) noexcept
  {
    //Satisfy both postconditions.
    this->gameStateChanged_ = true;
    this->gameState_ = gamestate;
  }
};

int main(int argc, char* argv[])
{
  pong::Game* game = pong::Game::getInstance();
  return game->run(argc, argv);
}
