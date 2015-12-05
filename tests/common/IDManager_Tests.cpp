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
#include "common/IDManager.hpp"
#include "gtest/gtest.h"

TEST(IDManager_Tests, UsesAllAvailableIDs)
{
  using pong::IDManager;

  using id_type = int16_t;

  IDManager<id_type> counter;

  for(id_type id = 0; ++id != 0;)
  {
    // We need to exhaust all the ids available.
    counter.get();
  }

  // We should have no ids left.
  EXPECT_EQ(0, counter.get());

  // Make room.
  counter.remove(1);

  // We should have at least room for one more.
  EXPECT_NE(0, counter.get());
}
