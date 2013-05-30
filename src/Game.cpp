/*!
 * \file Game.cpp
 * \brief Definitions for anything in Game.h
 */
#include "Game.h"
#include <stdexcept>
#include "scoped_init.hpp"
#include "GameStates/MenuGameState.h"
namespace pong
{
  std::shared_ptr<Game> Game::instance_ = nullptr;

  int Game::run(int argc, char* argv[])
  {
    this->initializeSDL();
    scoped_init<std::function<void ()> >
                               SDLinit(std::bind(&Game::uninitializeSDL, this));

    //Initially, go to the menu state.
    this->game_state_ = std::shared_ptr<GameState>(new MenuGameState);

    while(this->running_)
    {
      //If there is a game state waiting.
      if(this->game_state_to_be_)
      {
        //Change it
        this->game_state_ = this->game_state_to_be_;

        //Make sure this doesn't happen again.
        this->game_state_to_be_ = nullptr;
      }

      this->game_state_->update();
      this->game_state_->render(this->main_surface_);
      SDL_Flip(this->main_surface_);
    }

    //Return the error code.
    return 0;
  }
  void Game::initializeSDL()
  {
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
      throw std::runtime_error("Failed to initialize SDL");
    }
    //Satisfy the post condition.
    this->main_surface_ = SDL_SetVideoMode(1000, 1000, 32, SDL_HWSURFACE |
                                                           SDL_DOUBLEBUF);
    if(!this->main_surface_)
    {
      SDL_Quit();
      throw std::runtime_error("Failed to set SDL video mode.");
    }
  }
  void Game::uninitializeSDL() noexcept
  {
    //Free the surface, satisfying one post condition.
    SDL_FreeSurface(this->main_surface_);

    //Satisfy another post condition.
    this->main_surface_ = nullptr;

    //Quit sdl.
    SDL_Quit();
  }
};
int main(int argc, char* argv[])
{
  pong::Game* game = pong::Game::getInstance();
  return game->run(argc, argv);
}
