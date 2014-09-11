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
#include <tuple>
#include "common/parse/Value.hpp"
#include "common/parse/Tuple.hpp"
#include "common/parse/Generator.hpp"
#include "json/json.h"

TEST(Parse_Tests, ParseValue)
{
  Json::Value val(5);

  EXPECT_EQ(5, pong::parse::Value<int>::parse(val));

  val = "0";
  EXPECT_EQ("0", pong::parse::Value<std::string>::parse(val));
}
TEST(Parse_Tests, ParseTuple)
{
  Json::Value tup;
  tup.append(0);

  Json::Value subtup;
  subtup.append("0");
  subtup.append(0);

  tup.append(subtup);

  using pong::parse::Value;
  using pong::parse::Tuple;

  // {int, {string, int} }
  using parser = Tuple<Value<int>, Tuple<Value<std::string>, Value<int> > >;

  std::tuple<int, std::tuple<std::string, int> > result;
  std::get<0>(result) = 0;
  std::get<0>(std::get<1>(result)) = "0";
  std::get<1>(std::get<1>(result)) = 0;

  EXPECT_EQ(result, parser::parse(tup));
}
