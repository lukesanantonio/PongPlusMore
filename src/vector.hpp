/*!
 * \file vector.hpp
 * \brief Contains the o-so beautiful vector class.
 *
 * To be used for positions, velocities, anything you want!
 */
#ifndef ULTIMATE_PONG_VECTOR_HPP
#define ULTIMATE_PONG_VECTOR_HPP
namespace pong
{
  /*!
   * \brief Namespace for everything math related in pong.
   *
   * Collisions, vectors, etc.
   */
  namespace math
  {
    /*!
     * \brief The wonderful, wonderful vector class.
     */
    struct vector
    {
      int x = 0; //!< x (position | velocity | etc)
      int y = 0; //!< y (position | velocity | etc)
    };
  };
};
#endif
