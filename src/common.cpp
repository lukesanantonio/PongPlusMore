/*!
 * \file common.cpp
 * \brief Definitions to miscellaneous functions which anyone might find useful.
 */
#include "common.h"
#include <iostream>
#include <cstdlib>
#include <csignal>
namespace pong
{
  /*!
   * \brief Calls std::abort after displaying the why message!
   *
   * \note Does not return, ever!
   */
  [[noreturn]] void crash(const std::string& why)
  {
    std::cerr << "Crashing: " << why << std::endl;
    std::abort();
  }

  /*!
   * \brief Performs a runtime assertion of a surface.
   *
   * Should the test fail (when !surface) pong::crash is called with a message
   * containing the surface width and height!
   *
   * \param surface The surface to perform a runtime assertion on.
   * \param width The requested width during initialization...
   * \param height The requested height during initialization...
   */
  void runtime_assert_surface(SDL_Surface* surface, int width, int height)
  {
    if(surface) return;

    std::string message = "Failed to create SDL_Surface* with...";

    message += "\n\twidth: " + std::to_string(width);
    message += "\n\theight: " + std::to_string(height);

    crash(message);
  }

  /*!
   * \brief Performs a runtime assertion of a surface.
   *
   * Should the test fail, (when !surface) pong::crash is called with a message
   * saying some SDL_Surface* failed to be created!
   */
  void runtime_assert_surface(SDL_Surface* surface)
  {
    if(surface) return;

    crash("Failed to create SDL_Surface*!");
  }
};
