#include "Button.h"
namespace pong
{
  Button::Button(int x, int y, const std::string& label) : x_(x), y_(y),
                                                           label_(label)
  {}

  Button::Button() : Button(0, 0, "")
  {}
};
