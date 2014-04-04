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
#include "server/Quadtree.h"

TEST(Quadtree_Tests, ObjectMaxWorks)
{
  pong::Quadtree q({{0, 0}, 1000, 1000}, 1);

  using pong::Object; using pong::PhysicsType; using pong::id_type;
  id_type good = q.insertObject(Object{{{5, 5}, 20, 20}, PhysicsType::Paddle});
  id_type id = q.insertObject(Object{{{750, 750}, 20, 20}, PhysicsType::Ball});

  // We should have a root with four children.
  //          .- TODO:Make a type alias in Quadtree.
  using pong::Node; using pong::Node_Content;
  const Node<Node_Content>* root = q.root();
  EXPECT_EQ(4, root->children().size());

  // Erase an element. :/
  q.erase(id);

  // Check to make sure we are just one child again.
  EXPECT_EQ(0, root->children().size());
  // Make sure its the correct child. There should only be one id, so using
  // the zero indice, *should* work.
  EXPECT_EQ(good, root->get_data()->ids[0]);
}
