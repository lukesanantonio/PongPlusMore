#include "vector.h"
#include <cmath>
namespace pong
{
  namespace math
  {
    math::vector operator+(math::vector vec1, math::vector vec2)
    {
      return {vec1.x + vec2.x, vec1.y + vec2.y};
    }

    math::vector operator*(double scalar, math::vector vec1)
    {
      return {vec1.x * scalar, vec1.y * scalar};
    }

    math::vector normalize(math::vector vec)
    {
      //Calculate the length
      double length = math::length(vec);

      //We don't want to divide by zero!
      if(length == 0) return vec;

      //Divide each value in the vector by the length!
      vec.x /= length;
      vec.y /= length;

      //Return the new, normalized vector.
      return vec;
    }
    double length(math::vector vec)
    {
      double length = std::sqrt(std::pow(vec.x, 2) + std::pow(vec.y, 2));
      return length;
    }
  }
};
