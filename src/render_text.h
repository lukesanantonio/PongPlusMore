#ifndef ULTIMATE_PONG_RENDER_TEXT_H
#define ULTIMATE_PONG_RENDER_TEXT_H
#include <memory>
#include <SDL/SDL.h>
namespace pong
{
  SDL_Surface* render_text(const std::string& text, std::size_t pixel_size);
  void setupGrayscalePalette(SDL_Surface*& surface, std::size_t num_colors);
  void invertPalette(SDL_Surface*& surface);
};
#endif
