/*!
 * \file vector.h
 * \brief The header of a beautiful thing, a vector class.
 */
#ifndef ULTIMATE_PONG_VECTOR_H
#define ULTIMATE_PONG_VECTOR_H
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
      vector(double x, double y) noexcept : x(x), y(y){}
      /*!
       * \brief The default constructor, basically provides the default points
       * (0, 0) without having to be explicit, used for empty brace
       * initialization and more.
       */
      vector() noexcept : vector(0, 0){}
      /*!
       * \brief X value for anything imaginable.
       */
      double x = 0;
      /*!
       * \brief Y value for anything imaginable.
       */
      double y = 0;
    };

    math::vector operator+(math::vector vec1, math::vector vec2);

    math::vector operator*(double scalar, math::vector vec1);
    inline math::vector operator*(math::vector vec1, double scalar)
    {
      return scalar * vec1;
    }

    /*!
     * \brief Returns the unit vector of the passed in vector.
     */
    math::vector normalize(math::vector vec);

    /*!
     * \brief Returns the length of the passed in vector.
     */
    double length(math::vector vec);
  };
};
#endif
