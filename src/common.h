/*!
 * \file common.h
 * \brief Miscellaneous functions which anyone might find useful.
 */
#pragma once
#include <string>
#include "SDL.h"
namespace pong
{
  [[noreturn]] void crash(const std::string& why);

  void runtime_assert_surface(SDL_Surface* surface, int width, int height);
  void runtime_assert_surfaec(SDL_Surface* surface);
}
