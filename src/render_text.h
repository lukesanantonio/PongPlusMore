/*!
 * \file render_text.h
 * \brief Basically provides the crap needed to render text to an SDL_Surface*.
 */
#ifndef ULTIMATE_PONG_RENDER_TEXT_H
#define ULTIMATE_PONG_RENDER_TEXT_H
#include <memory>
#include <SDL/SDL.h>
namespace pong
{
  /*!
   * \brief Renders a string of text to an SDL_Surface*.
   *
   * \note Should be used sparsly as each call requires a new FT_Library.
   *
   * \param text String of text to render.
   * \param pixel_size Passed as the pixel height to `FT_Set_Pixel_Sizes()`.
   *
   * \returns An `SDL_Surface` with minimum bounds possible, of the text passed
   * in.
   *
   * \note The returned `SDL_Surface` which needs to be manually deleted with a
   * call to `SDL_FreeSurface()`.
   *
   * \throws A runtime error if anything goes wrong.
   */
  SDL_Surface* render_text(const std::string& text, std::size_t pixel_size);
  
  /*!
   * \brief Blesses an SDL_Surface with a grayscale palette.
   *
   * Now each byte in an SDL_Surface's pixels can refer to grayscale intensity,
   * rather than some other homebrew, confusing crap that might be set up
   * otherwise.
   *
   * \param surface A reference to a surface of which to add a palette.
   * \param num_colors The amount of shades of gray.
   */
  void setupGrayscalePalette(SDL_Surface*& surface, std::size_t num_colors);
  
  /*!
   * \brief Inverts the palette of an SDL_Surface.
   *
   * \param surface whose palette is to be inverted.
   */
  void invertPalette(SDL_Surface*& surface);
};
#endif
