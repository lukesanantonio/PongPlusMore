#ifndef ULTIMATE_PONG_GAME_H
#define ULTIMATE_PONG_GAME_H
#include <memory>
#include <SDL/SDL.h>
#include "GameState.h"
namespace pong
{
  class Game
  {
  public:
    inline static Game* getInstance();
    int run(int argc, char* argv[]);
    void setGameState(std::shared_ptr<GameState> gamestate) noexcept;
    inline void quitGame() noexcept;
  private:
    Game() noexcept = default;
    ~Game() noexcept = default;
    Game(const Game&) = delete;
    Game(Game&&) = delete;
    Game& operator=(const Game&) = delete;
    Game& operator=(Game&&) = delete;
    static std::shared_ptr<Game> instance_;
    SDL_Surface* mainSurface_ = nullptr;
    void initializeSDL();
    void uninitializeSDL() noexcept;
    std::shared_ptr<GameState> gameState_;
    bool gameStateChanged_ = false;
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
};
#endif
