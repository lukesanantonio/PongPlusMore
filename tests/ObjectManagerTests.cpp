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
#include "server/ObjectManager.h"
#include "gtest/gtest.h"

TEST(ObjectManagerTest, ObjectInsertionWorks)
{
  using pong::ObjectManager;
  ObjectManager objs;

  static_assert(std::is_unsigned<ObjectManager::id_type>::value,
                "ID type unsigned, overflow undefined...");

  for(ObjectManager::id_type id = 0; ++id != 0;)
  {
    // We need to exhaust all the ids available.
    objs.makePaddle({{0,0}, 100, 100});
  }

  // We should have absolutely no ids left.
  EXPECT_EQ(0, objs.makeBall({{5,5}, 100, 100}));

  // Now give ourselves room.
  objs.erase(objs.begin()->first);

  EXPECT_NE(0, objs.makeBall({{5,5}, 100, 200}));
}
