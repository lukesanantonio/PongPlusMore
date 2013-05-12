/*!
 * \file vector.hpp
 * \brief The header of a beautiful thing, a vector class.
 */
#ifndef ULTIMATE_PONG_VECTOR_HPP
#define ULTIMATE_PONG_VECTOR_HPP
namespace pong
{
  /*!
   * \brief A namespace for all math related pong things.
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
       * \brief Constructor used for brace initialization with two values,
       * regular construction with two values, etc.
       */
      vector(int x, int y) : x(x), y(y){}
      /*!
       * \brief The default constructor, basically provides the default points
       * (0, 0) without having to be explicit, used for empty brace
       * initialization and more.
       */
      vector() : vector(0, 0){}
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
