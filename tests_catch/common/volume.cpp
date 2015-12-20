/*
 * uGlue - Glue many languages together into a whole with ukernel-inspired RPC.
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
#include <catch.hpp>
#include "common/volume.h"

TEST_CASE("Find direction", "[volume]")
{
  using namespace ug;

  Vec<double> test_vector = {0, 1};
  CHECK(VolumeSide::Bottom == find_direction(test_vector));

  test_vector = {1, -1};
  CHECK((VolumeSide::Right | VolumeSide::Top) == find_direction(test_vector));

  test_vector = {-1, 1};
  CHECK((VolumeSide::Bottom | VolumeSide::Left) == find_direction(test_vector));
}
TEST_CASE("Constrain", "[volume]")
{
  using namespace ug;

  Vec<double> test_vector = {0.0, 0.0};
  CHECK(test_vector == constrain<double>({0, -1}, VolumeSide::Top));
  CHECK(test_vector == constrain<double>({-1, 0}, VolumeSide::Left));
  CHECK(test_vector == constrain<double>({-1, -1},
        VolumeSide::Left | VolumeSide::Top));

  test_vector = {-1, -1};
  CHECK(test_vector == constrain<double>({-1, -1},
            VolumeSide::Right | VolumeSide::Bottom));
  test_vector = {0, 1};
  CHECK(test_vector == constrain<double>({1, 1},
            VolumeSide::Right | VolumeSide::Top));
}
TEST_CASE("Flip", "[volume]")
{
  using namespace ug;

  CHECK(VolumeSide::Top == flip(VolumeSide::Bottom));

  CHECK((VolumeSide::Left | VolumeSide::Top) ==
            flip(VolumeSide::Right | VolumeSide::Bottom));

  CHECK(VolumeSide::Bottom == flip(flip(VolumeSide::Bottom)));

  CHECK((VolumeSide::Top | VolumeSide::Left) ==
            flip(VolumeSide::Bottom | VolumeSide::Right));
}

TEST_CASE("Intersecting", "[volume]")
{
  using namespace ug;

  Volume<int> v1 = {{0, 0}, 10, 10};
  Volume<int> v2 = {{5, 5}, 10, 10};

  CHECK(intersecting(v1, v2));
  v1.pos = {10, 10};
  CHECK(intersecting(v1, v2));
  v1.pos = {15, 15};
  CHECK_FALSE(intersecting(v1, v2));
  v2.pos = {-5, -5};
  CHECK_FALSE(intersecting(v1, v2));
}
TEST_CASE("Extending sides", "[volume]")
{
  using namespace ug;

  // Self check.
  Volume<int> bounds = {{0, 0}, 10, 500};
  CHECK(VolumeSide::None == extending_sides(bounds, bounds));

  Volume<int> obj;

  // Test one side.
  obj = {{-5, 0}, 10, 5};
  CHECK(VolumeSide::Left == extending_sides(obj, bounds));

  // Test multiple returned sides.
  obj = {{-5, -5}, 10, 10};
  CHECK((VolumeSide::Left | VolumeSide::Top) == extending_sides(obj, bounds));
}
TEST_CASE("Closest side", "[volume]")
{
  using namespace ug;

  Volume<int> v1 = {{0,0}, 5, 5};
  Volume<int> v2 = {{5,0}, 5, 5};
  CHECK(VolumeSide::Right == closest_side(v1, v2));

  v2.pos = {-5, 0};
  CHECK(VolumeSide::Left == closest_side(v1, v2));
  v2.pos = {-4, 3};
  CHECK(VolumeSide::Left == closest_side(v1, v2));

  v1.pos = {-3, -2};
  CHECK(VolumeSide::Bottom == closest_side(v1, v2));
}
TEST_CASE("Inside side", "[volume]")
{
  using namespace ug;

  // An volume should always be inside itself.
  Volume<int> box = {{0, 0}, 1000, 1000};
  CHECK(inside(box, box));

  // Sanity check #1.
  Volume<int> obj = {{10, 10}, 10, 10};
  CHECK(inside(box, obj));
  CHECK_FALSE(inside(obj, box));

  // Sanity check #2.
  obj.pos = {-5, 0};
  CHECK_FALSE(inside(box, obj));
  CHECK_FALSE(inside(obj, box));
}
TEST_CASE("Outside snap", "[volume]")
{
  using namespace ug;

  Volume<int> v1 = {{0,0}, 10, 10};
  Volume<int> v2 = {{50, 50}, 10, 10};

  // Sanity check #1
  v2.pos += outside_snap(v2, VolumeSide::Right, v1);
  CHECK(10 == v2.pos.x);
  CHECK_FALSE(intersecting(v1, v2));

  // Sanity check #2
  v1.pos += outside_snap(v1, VolumeSide::Left, v2);
  CHECK(0 == v1.pos.x);
  CHECK_FALSE(intersecting(v1, v2));

  // Sanity check #3
  v2.pos.x = 50;
  v1.pos += outside_snap(v1, VolumeSide::Left, v2);
  // Make sure the right side of v1 is aligned to the left side of v2.
  CHECK(40 == v1.pos.x);
  // They shouldn't be intersecting
  CHECK_FALSE(intersecting(v1, v2));
  // And the right side of v1 should be the closest to v2.
  CHECK(VolumeSide::Right == closest_side(v1, v2));

  // Test multiple volume sides.
  v1.pos += outside_snap(v1, VolumeSide::Right | VolumeSide::Top, v2);
  const Vec<double> test_value = {60, 40};
  CHECK(test_value == v1.pos);
}
TEST_CASE("Inside snap", "[volume]")
{
  using namespace ug;

  Volume<int> box = {{0,0}, 100, 100};
  Volume<int> obj = {{50,50}, 10, 10};

  // Sanity check
  obj.pos += inside_snap(obj, VolumeSide::Top, box);
  CHECK(0 == obj.pos.y);
  CHECK(inside(box, obj));

  // Test multiple volume sides.
  obj.pos += inside_snap(obj, VolumeSide::Bottom | VolumeSide::Right, box);
  const Vec<double> test_value = {90, 90};
  CHECK(test_value == obj.pos);
  CHECK(inside(box, obj));
}
TEST_CASE("Volume quads", "[volume]")
{
  using namespace ug;

  // One test'll do. Real simple.
  Volume<int> bounds = {{0,0}, 500, 500};
  std::array<Volume<int>, 4> quads = volume_quads(bounds);

  Volume<int> test_volume = {{0, 0}, 250, 250};
  CHECK(test_volume == quads[0]);

  test_volume.pos = {250, 0};
  CHECK(test_volume == quads[1]);

  test_volume.pos = {0, 250};
  CHECK(test_volume == quads[2]);

  test_volume.pos = {250, 250};
  CHECK(test_volume == quads[3]);
}
