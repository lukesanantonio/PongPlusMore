/*
 * ppm - pong plus more - a pong clone full of surprises written with c++11.
 * copyright (c) 2013  luke san antonio
 *
 * you can contact me (luke san antonio) at lukesanantonio@gmail.com!
 *
 * this program is free software: you can redistribute it and/or modify
 * it under the terms of the gnu general public license as published by
 * the free software foundation, either version 3 of the license, or
 * (at your option) any later version.
 *
 * this program is distributed in the hope that it will be useful,
 * but without any warranty; without even the implied warranty of
 * merchantability or fitness for a particular purpose.  see the
 * gnu general public license for more details.
 *
 * you should have received a copy of the gnu general public license
 * along with this program.  if not, see <http://www.gnu.org/licenses/>.
 */
#include "common/volume.h"
#include "gtest/gtest.h"

TEST(Volume_Tests, extending_sides)
{
  using namespace pong;

  Volume bounds = {{0, 0}, 10, 500};
  EXPECT_EQ(VolumeSide::None, extending_sides(bounds, bounds));

  Volume obj;

  obj = {{-5, 0}, 10, 5};
  EXPECT_EQ(VolumeSide::Left, extending_sides(obj, bounds));

  obj = {{-5, -5}, 10, 10};
  EXPECT_EQ(VolumeSide::Left | VolumeSide::Top, extending_sides(obj, bounds));
}
