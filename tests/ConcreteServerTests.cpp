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
#include "server/ConcreteServer.h"
#include "gtest/gtest.h"

//First PpM test; dedicated to commit: 6b934366d7e3529ae09b34a316aa81d9e9a05e9b
TEST(ConcreteServerTest, MakesPaddles)
{
  pong::PaddleID first = 0, second = 0, third = 0;
  pong::ConcreteServer server(1000, 1000);

  //Get three paddle ids.
  first = server.makePaddle();
  second = server.makePaddle();
  third = server.makePaddle();

  //All three paddle ids should be unique.
  EXPECT_NE(first, second);
  EXPECT_NE(first, third);
  EXPECT_NE(second, third);

  //Make sure the third paddle id is invalid.
  EXPECT_EQ(0, third);
}
