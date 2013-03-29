/*!
 * \file vector.hpp
 * \brief The header of a beautiful thing, a vector class.
 */
#ifndef ULTIMATE_PONG_VECTOR_HPP
#define ULTIMATE_PONG_VECTOR_HPP
namespace pong
{
  /*!
   * A namespace for all math related pong things.
   *
   * That could be collision detection, vectors, velocities, positions, etc.
   */
  namespace math
  {
    /*!
     * \brief A simple vector class.
     */
    struct vector
    {
      /*!
       * \brief X value for anything imaginable.
       */
      int x = 0;
      /*!
       * \brief Y value for anything imaginable.
       */
      int y = 0;
    };
  };
};
#endif
