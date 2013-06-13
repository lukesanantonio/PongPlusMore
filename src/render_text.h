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
  SDL_Surface* render_text(const std::string& text, std::size_t pixel_size,
                           SDL_Color text_color, SDL_Color background_color);

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
   * \brief Gets the point which should be the location of an object of
   * `object_length` with the bounds which begin at `begin` and go on for
   * `length`.
   *
   * \param begin The location where the line, etc begins.
   * \param length How far this line, etc, goes for, after `begin`.
   * \param object_length The length of the object which needs to be centered.
   *
   * \return The 1d point which should be the position of an object of length
   * `object_length` if it needs to be centered in a line starting at point
   * `begin` going on for `length` amount.
   *
   * \note Example: If begin==100, length==100, and object_length==50. This
   * function returns 125. This would be the x or y point for on object of
   * length 50 which needs to be centered within the bounds 100-200.
   *
   * \note Hope this is clear. Otherwise check out the source code. If you
   * don't want to look at the source code the algorithm is exactly:
   * \code begin + (length / 2) - (object_length / 2) \endcode
   * This is what is returned.
   */
  inline int center(int begin, std::size_t length, std::size_t object_length)
  {
    return begin + (length / 2) - (object_length / 2);
  }

  /*!
   * \brief Generates a rectange SDL_Surface of any color, width, or height.
   *
   * \returns An SDL_Create(d)RGBSurface() which **needs to be
   * SDL_FreeSurface()'d.**
   */
  SDL_Surface* generateRectangle(std::size_t width, std::size_t height,
                                 SDL_Color color);
};
#endif
