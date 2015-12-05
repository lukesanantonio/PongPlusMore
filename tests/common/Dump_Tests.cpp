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
#include "gtest/gtest.h"
#include "common/vector.h"
#include "common/volume.h"
#include "common/dump/Value.hpp"
#include "common/dump/Object.hpp"

TEST(Dump_Tests, DumpValue)
{
  using dump_t = pong::dump::Value<int>;

  Json::Value result(5);
  EXPECT_EQ(result, dump_t::dump(5));

  result = 10;
  EXPECT_EQ(result, dump_t::dump(10));
}

TEST(Dump_Tests, DumpVector)
{
  using dump_t = pong::dump::Object<pong::math::vector<double> >;

  Json::Value result;
  result["x"] = 5.0;
  result["y"] = 10.0;

  EXPECT_EQ(result, dump_t::dump({5.0, 10.0}));
}

TEST(Dump_Tests, DumpVolume)
{
  using dump_t = pong::dump::Object<pong::Volume>;

  Json::Value result;
  result["Width"] = 500.0;
  result["Height"] = 50.0;
  result["Position"]["x"] = 5.0;
  result["Position"]["y"] = 15.0;

  EXPECT_EQ(result, dump_t::dump({{5,15}, 500, 50}));
}
