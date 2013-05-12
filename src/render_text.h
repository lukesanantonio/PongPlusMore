/*!
 * \file render_text.h
 * \brief File declaring font related procedures.
 */
#ifndef ULTIMATE_PONG_RENDER_TEXT_H
#define ULTIMATE_PONG_RENDER_TEXT_H
#include <memory>
#include <SDL/SDL.h>
namespace pong
{
  /*!
   * \brief Renders a string of text at a specified pixel size.
   *
   * \returns An SDL_Surface pointer with the minimum bounds of the text at the
   * pixel size passed in.
   * \note The SDL_Surface* returned must be `SDL_FreeSurface()`'d manually.
   *
   * \param text A string of text to render.
   * \param pixel_size The height in pixels to render the string of text at.
   */
  SDL_Surface* render_text(const std::string& text, std::size_t pixel_size);

  /*!
   * \brief Sets up a grayscale palette for an SDL_Surface.
   *
   * \param surface Surface to modify in-place.
   * \param num_colors The amount of shades of gray to generate for the palette.
   * \note For an 8-bit surface, this should not exceed 256.
   */
  void setupGrayscalePalette(SDL_Surface*& surface, std::size_t num_colors);

  /*!
   * \brief Inverts the palette of the passed in surface.
   *
   * \param surface Surface whose palette to invert.
   */
  void invertPalette(SDL_Surface*& surface);

  /*!
   * \brief Gets the point which is the center of begin to length.
   *
   * \param begin The location where the line, etc begins.
   * \param length How far this line, etc, goes for, after `begin`.
   *
   * \return The 1d center of point `begin` going on for `length` amount.
   *
   * \note This is just a small helper, nothing special happens.
   */
  inline int center(int begin, std::size_t length)
  {
    return begin + (length - begin) / 2;
  }
};
#endif
