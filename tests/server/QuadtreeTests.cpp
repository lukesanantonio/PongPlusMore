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
#include <gtest/gtest.h>
#include "common/Volume.h"
#include "server/Quadtree.h"

TEST(NodeTests, FindFirstChildWorks)
{
  pong::Node<int> root;

  decltype(root)* first_child = root.push_child()->push_child();
  root.push_child();
  root.push_child()->push_child()->push_child();

  EXPECT_EQ(first_child, pong::find_first_child(&root));
}
TEST(NodeTests, FindLastChildWorks)
{
  pong::Node<std::vector<int> > root;

  root.push_child()->push_child();
  root.push_child();
  decltype(root)* last_child = root.push_child()->push_child()->push_child();

  EXPECT_EQ(last_child, pong::find_last_child(&root));
}
TEST(NodeTests, NodeIteratorWorks)
{
  struct none {};
  using node_type = pong::Node<none>;
  node_type root;

  std::vector<node_type*> expected_path;

  node_type* first_child = root.push_child();
  expected_path.push_back(first_child->push_child());
  expected_path.push_back(first_child->push_child());
  expected_path.push_back(root.push_child());

  std::vector<node_type*> actual_path;

  for(node_type& n : root)
  {
    actual_path.push_back(&n);
  }

  EXPECT_EQ(expected_path, actual_path);
}
