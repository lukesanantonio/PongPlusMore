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
   * \param pixel_size Passed as the pixel height to `FT_Set_Pixel_Sizes()`.
   * \param text String of text to render.
   *
   * \returns An `SDL_Surface` with minimum bounds possible, of the text passed
   * in.
   *
   * \note The returned `SDL_Surface` which needs to be manually deleted with a
   * call to `SDL_FreeSurface()`.
   *
   * \throws A runtime error if anything goes wrong.
   */
  SDL_Surface* render_text(std::size_t pixel_size, const std::string& text);
};
#endif
