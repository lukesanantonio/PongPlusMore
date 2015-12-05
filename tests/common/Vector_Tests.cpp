/*
 * uGlue - Glue many languages together into a whole with ukernel-inspired RPC.
 * Copyright (C) 2013  Luke San Antonio
 *
 * You can contact me (Luke San Antonio) at lukesanantonio@gmail.com!
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "common/vector.h"
#include "gtest/gtest.h"
#include <cstdint>

TEST(Vector_Tests, Equality)
{
  using pong::math::vector;

  vector<uint16_t> vecui(5, 5);
  EXPECT_EQ(vecui, vecui);

  vector<double> vecd(5.5, 5.5);
  EXPECT_EQ(vecd, vecd);
}
TEST(Vector_Tests, Rotation)
{
  namespace pm = pong::math;

  auto calc = pm::rotate(pm::vector<int16_t>(1,0), 3.14159);
  pm::vector<int16_t> expected(-1, 0);

  EXPECT_EQ(expected, calc);
}
