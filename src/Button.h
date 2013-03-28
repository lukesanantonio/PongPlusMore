#ifndef ULTIMATE_PONG_BUTTON_H
#define ULTIMATE_PONG_BUTTON_H
#include <string>
#include <SDL/SDL.h>
namespace pong
{
  class Button
  {
  public:
    Button(int x, int y, const std::string& label);
    Button();
    ~Button() noexcept = default;
    Button(const Button&) = delete;
    Button(Button&&) = delete;
    Button& operator=(const Button&) = delete;
    Button& operator=(Button&&) = delete;
  private:
    SDL_Surface* image_;
    int x_;
    int y_;
    std::string label_;
  };
};
#endif
