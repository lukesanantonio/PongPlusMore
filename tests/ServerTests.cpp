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
 *
 * \file ServerTests.cpp
 * \brief Tests of various server/ specific stuff, but which doesn't fit in
 * other test files.
 */

#include "server/util.h"
#include "server/World.h"
#include "gtest/gtest.h"

TEST(ServerTests, findObjectByID)
{
  pong::World world;

  world.paddles.emplace_back(0);
  world.paddles.emplace_back(2);
  world.paddles.emplace_back(5);
  world.paddles.emplace_back(1);

  pong::id_type id = 5;

  using std::begin; using std::end;
  auto iter = pong::findObjectByID(begin(world.paddles),
                                   end(world.paddles), id);

  ASSERT_NE(end(world.paddles), iter);
  EXPECT_EQ(id, iter->id());

  id = 6;

  iter = pong::findObjectByID(begin(world.paddles),
                              end(world.paddles), id);
  EXPECT_EQ(end(world.paddles), iter);
}
