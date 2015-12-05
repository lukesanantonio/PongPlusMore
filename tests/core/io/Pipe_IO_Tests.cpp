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
#include "core/common/utility.h"
#include "core/io/external_io.h"

TEST(Pipe_IO_Tests, Counterpart_Is_Reversible)
{
  pong::Pipe_IO pipe;

  EXPECT_EQ(&pipe, &pipe.counterpart().counterpart());
  EXPECT_NE(&pipe, &pipe.counterpart().counterpart().counterpart());
}

TEST(Pipe_IO_Tests, Write_To_Counterpart_Works)
{
  using namespace pong::literals;

  pong::Pipe_IO pipe;
  auto test_str = "..."_buf;

  bool worked = false;
  pipe.counterpart().set_read_callback([&](std::vector<char> const& buf)
  {
    worked = (buf == test_str);
  });

  pipe.write(test_str);
  pipe.step();

  EXPECT_TRUE(worked);
}
TEST(Pipe_IO_Tests, Read_From_Counterpart_Works)
{
  using namespace pong::literals;

  pong::Pipe_IO pipe;
  auto test_str = "..."_buf;

  bool worked = false;
  pipe.set_read_callback([&](std::vector<char> const& buf)
  {
    worked = (buf == test_str);
  });

  pipe.counterpart().write(test_str);
  pipe.step();

  EXPECT_TRUE(worked);
}
