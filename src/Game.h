#ifndef ULTIMATE_PONG_H
#define ULTIMATE_PONG_H
#include <memory>
namespace pong
{
  class Game
  {
  public:
    inline static Game* getInstance();
    int run(int argc, char* argv[]);
  private:
    Game() = default;
    ~Game() = default;
    
    Game(const Game&) = delete;
    Game(Game&&) = delete;
    
    Game& operator=(const Game&) = delete;
    Game& operator=(Game&&) = delete;
    
    static std::shared_ptr<Game> instance_;
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
