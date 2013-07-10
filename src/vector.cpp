#include "vector.h"
#include <cmath>
namespace pong
{
  namespace math
  {
    math::vector normalize(math::vector vec)
    {
      double length = math::length(vec);
      vec.x /= length;
      vec.y /= length;
      return vec;
    }
    double length(math::vector vec)
    {
      double length = std::sqrt(std::pow(vec.x, 2) + std::pow(vec.y, 2));
      return length;
    }
  }
};
