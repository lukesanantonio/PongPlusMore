/*!
 * \file vec.hpp
 * \brief Contains the o-so beautiful vector class.
 *
 * To be used for positions, velocities, anything you want!
 */
#ifndef ULTIMATE_PONG_VEC_HPP
#define ULTIMATE_PONG_VEC_HPP
namespace pong
{
  /*!
   * \brief The wonderful, wonderful vector class.
   */
  struct vec
  {
    int x = 0; //!< x (position | velocity | etc)
    int y = 0; //!< y (position | velocity | etc)
  };
};
#endif
