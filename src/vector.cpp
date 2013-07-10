#include "vector.h"
#include <cmath>
namespace pong
{
  namespace math
  {
    math::vector normalize(math::vector vec)
    {
      //Calculate length.
      float length = std::sqrt(std::pow(vec.x, 2) + std::pow(vec.y, 2));
      vec.x /= length;
      vec.y /= length;
      return vec;
    }
  }
};
