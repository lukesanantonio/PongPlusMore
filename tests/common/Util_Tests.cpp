/*
 * PpM - Pong Plus More - A pong clone full of surprises written with C++11.
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
#include "common/util.h"

TEST(Util_Tests, VectorCast)
{
  std::vector<float> floats = { 5.135, 6.123, 7.23 };

  // Test using implicit conversion.
  std::vector<int> ints = pong::vector_cast<int>(floats);
  std::vector<int> expected = { 5, 6, 7 };
  EXPECT_EQ(expected, ints);

  // Test using the explicit conversion.
  std::vector<std::string> strings =
     pong::vector_cast<std::string>(floats,
                  [](float f) { return std::to_string(static_cast<int>(f)); });
  std::vector<std::string> expected_strings{ "5", "6", "7" };
  EXPECT_EQ(expected_strings, strings);
}
TEST(Util_Tests, GetDataVector)
{
  using pong::get_data_vector;

  std::vector<std::unique_ptr<int> > ints;
  for(int i = 0; i < 10; ++i)
  {
    ints.push_back(std::make_unique<int>(i));
  }

  std::vector<int*> result = get_data_vector<int*>(ints);
  std::vector<const int*> const_result = get_data_vector<const int*>(ints);

  auto compare = [](const std::unique_ptr<int>& p, const int* pi)
  { return p.get() == pi; };

  using std::begin;
  EXPECT_TRUE(std::equal(begin(ints), end(ints),
                         begin(result), compare));
  EXPECT_TRUE(std::equal(begin(ints), end(ints),
                         begin(const_result), compare));
}
