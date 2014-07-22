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

TEST(Volume_Tests, flip)
{
  using namespace pong;

  EXPECT_EQ(VolumeSide::Top, flip(VolumeSide::Bottom));

  EXPECT_EQ(VolumeSide::Left | VolumeSide::Top,
            flip(VolumeSide::Right | VolumeSide::Bottom));

  EXPECT_EQ(VolumeSide::Bottom, flip(flip(VolumeSide::Bottom)));

  EXPECT_EQ(VolumeSide::Top | VolumeSide::Left,
            flip(VolumeSide::Bottom | VolumeSide::Right));
}

TEST(Volume_Tests, intersecting)
{
  using namespace pong;

  Volume v1 = {{0, 0}, 10, 10};
  Volume v2 = {{5, 5}, 10, 10};

  EXPECT_TRUE(intersecting(v1, v2));
  v1.pos = {10, 10};
  EXPECT_TRUE(intersecting(v1, v2));
  v1.pos = {15, 15};
  EXPECT_FALSE(intersecting(v1, v2));
  v2.pos = {-5, -5};
  EXPECT_FALSE(intersecting(v1, v2));
}
TEST(Volume_Tests, extending_sides)
{
  using namespace pong;

  // Self check.
  Volume bounds = {{0, 0}, 10, 500};
  EXPECT_EQ(VolumeSide::None, extending_sides(bounds, bounds));

  Volume obj;

  // Test one side.
  obj = {{-5, 0}, 10, 5};
  EXPECT_EQ(VolumeSide::Left, extending_sides(obj, bounds));

  // Test multiple returned sides.
  obj = {{-5, -5}, 10, 10};
  EXPECT_EQ(VolumeSide::Left | VolumeSide::Top, extending_sides(obj, bounds));
}
TEST(Volume_Tests, closest_side)
{
  using namespace pong;

  Volume v1 = {{0,0}, 5, 5};
  Volume v2 = {{5,0}, 5, 5};
  EXPECT_EQ(VolumeSide::Right, closest_side(v1, v2));

  v2.pos = {-5, 0};
  EXPECT_EQ(VolumeSide::Left, closest_side(v1, v2));
  v2.pos = {-4, 3};
  EXPECT_EQ(VolumeSide::Left, closest_side(v1, v2));

  v1.pos = {-3, -2};
  EXPECT_EQ(VolumeSide::Bottom, closest_side(v1, v2));
}
TEST(Volume_Tests, inside)
{
  using namespace pong;

  // An volume should always be inside itself.
  Volume box = {{0, 0}, 1000, 1000};
  EXPECT_TRUE(inside(box, box));

  // Sanity check #1.
  Volume obj = {{10, 10}, 10, 10};
  EXPECT_TRUE(inside(box, obj));
  EXPECT_FALSE(inside(obj, box));

  // Sanity check #2.
  obj.pos = {-5, 0};
  EXPECT_FALSE(inside(box, obj));
  EXPECT_FALSE(inside(obj, box));
}
TEST(Volume_Tests, outside_snap)
{
  using namespace pong;

  Volume v1 = {{0,0}, 10, 10};
  Volume v2 = {{50, 50}, 10, 10};

  // Sanity check #1
  v2.pos += outside_snap(v2, VolumeSide::Right, v1);
  EXPECT_EQ(10, v2.pos.x);
  EXPECT_FALSE(intersecting(v1, v2));

  // Sanity check #2
  v1.pos += outside_snap(v1, VolumeSide::Left, v2);
  EXPECT_EQ(0, v1.pos.x);
  EXPECT_FALSE(intersecting(v1, v2));

  // Sanity check #3
  v2.pos.x = 50;
  v1.pos += outside_snap(v1, VolumeSide::Left, v2);
  // Make sure the right side of v1 is aligned to the left side of v2.
  EXPECT_EQ(40, v1.pos.x);
  // They shouldn't be intersecting
  EXPECT_FALSE(intersecting(v1, v2));
  // And the right side of v1 should be the closest to v2.
  EXPECT_EQ(VolumeSide::Right, closest_side(v1, v2));

  // Test multiple volume sides.
  v1.pos += outside_snap(v1, VolumeSide::Right | VolumeSide::Top, v2);
  const math::vector<double> test_value = {60, 40};
  EXPECT_EQ(test_value, v1.pos);
}
TEST(Volume_Tests, inside_snap)
{
  using namespace pong;

  Volume box = {{0,0}, 100, 100};
  Volume obj = {{50,50}, 10, 10};

  // Sanity check
  obj.pos += inside_snap(obj, VolumeSide::Top, box);
  EXPECT_EQ(0, obj.pos.y);
  EXPECT_TRUE(inside(box, obj));

  // Test multiple volume sides.
  obj.pos += inside_snap(obj, VolumeSide::Bottom | VolumeSide::Right, box);
  const math::vector<double> test_value = {90, 90};
  EXPECT_EQ(test_value, obj.pos);
  EXPECT_TRUE(inside(box, obj));
}
TEST(Volume_Tests, volume_quads)
{
  using namespace pong;

  // One test'll do. Real simple.
  Volume bounds = {{0,0}, 500, 500};
  std::array<Volume, 4> quads = volume_quads(bounds);

  Volume test_volume = {{0, 0}, 250, 250};
  EXPECT_EQ(test_volume, quads[0]);

  test_volume.pos = {250, 0};
  EXPECT_EQ(test_volume, quads[1]);

  test_volume.pos = {0, 250};
  EXPECT_EQ(test_volume, quads[2]);

  test_volume.pos = {250, 250};
  EXPECT_EQ(test_volume, quads[3]);
}
