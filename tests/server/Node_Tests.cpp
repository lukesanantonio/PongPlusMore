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
#include "server/Node.hpp"

TEST(Node_Tests, FindFirstChildWorks)
{
  pong::Node<int> root;

  decltype(root)* first_child = root.push_child()->push_child();
  root.push_child();
  root.push_child()->push_child()->push_child();

  EXPECT_EQ(first_child, pong::find_first_child(&root));
}
TEST(Node_Tests, FindLastChildWorks)
{
  pong::Node<std::vector<int> > root;

  root.push_child()->push_child();
  root.push_child();
  decltype(root)* last_child = root.push_child()->push_child()->push_child();

  EXPECT_EQ(last_child, pong::find_last_child(&root));
}
TEST(Node_Tests, NodeIteratorWorks)
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
TEST(Node_Tests, NodeIteratorIsEqualToNodePointer)
{
  struct none {};
  using node_type = pong::Node<none>;
  node_type root;

  node_type* expected = root.push_child();
  node_type* next = root.push_child();

  EXPECT_EQ(expected, root.begin());
  EXPECT_EQ(next, ++root.begin());
}
TEST(Node_Tests, NodeIteratorPastTheEndWorks)
{
  // According to the standard container.back() should be the same as
  // { iterator tmp = a.end(); --tmp; return *tmp; }
  // Although the Node class doesn't have a back function its still good
  // to follow that you-can-deincrement-the-past-the-end iterator practice.
  struct none {};
  using node_type = pong::Node<none>;
  node_type r;

  r.push_child()->push_child();
  node_type* expected = r.push_child();

  auto iter = r.cend(); --iter;
  EXPECT_EQ(expected, iter);
}
TEST(Node_Tests, ChildRemovalWorks)
{
  struct none {};
  using node_type = pong::Node<none>;
  node_type r;

  node_type* c1 = r.push_child();
  node_type* c2 = r.push_child();

  std::vector<node_type*> expected_path;
  expected_path.push_back(c2->push_child());
  expected_path.push_back(c2->push_child());

  EXPECT_FALSE(r.remove_child(nullptr));
  EXPECT_TRUE(r.remove_child(c1));

  std::vector<node_type*> actual_path;
  for(node_type& n : r)
  {
    actual_path.push_back(&n);
  }
  EXPECT_EQ(expected_path, actual_path);
}
