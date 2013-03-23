#include "Game.h"
namespace pong
{
  std::shared_ptr<Game> Game::instance_ = nullptr;
  
  int Game::run(int argc, char* argv[])
  {
    return 0;
  }
};

int main(int argc, char* argv[])
{
  pong::Game* game = pong::Game::getInstance();
  return game->run(argc, argv);
}
