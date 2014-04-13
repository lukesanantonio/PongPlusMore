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

TEST(Quadtree_Tests, ErasingWorks)
{
  using pong::Quadtree;
  Quadtree q({{0, 0}, 1000, 1000}, 1);

  using pong::ObjectManager;
  const ObjectManager& objs = q.obj_manager();

  using pong::Volume;
  q.makeBall(Volume{{0,0}, 1, 1});
  q.makeBall(Volume{{10,10}, 1, 1});
  q.makeBall(Volume{{20,20}, 1, 1});
  q.makeBall(Volume{{30,30}, 1, 1});
  q.makeBall(Volume{{40,40}, 1, 1});

  EXPECT_EQ(5, objs.size());
  q.erase(objs.begin());
  EXPECT_EQ(4, objs.size());

  auto erase_end = objs.begin();
  std::advance(erase_end, 2);
  q.erase(objs.begin(), erase_end);
  EXPECT_EQ(2, objs.size());

  q.erase(objs.begin()->first);
  EXPECT_EQ(1, objs.size());
}
TEST(Quadtree_Tests, ObjectMaxWorks)
{
  using pong::Quadtree;
  Quadtree q({{0, 0}, 1000, 1000}, 1);

  using pong::Object; using pong::PhysicsType; using pong::id_type;
  id_type good = q.makePaddle({{5, 5}, 20, 20});
  id_type id = q.makeBall({{750, 750}, 20, 20});

  // We should have a root with four children.
  const Quadtree::node_type* root = q.root();
  EXPECT_EQ(4, root->children().size());

  // Erase an element. :/
  q.erase(id);

  // Check to make sure we are just one child again.
  EXPECT_EQ(0, root->children().size());
  // Make sure its the correct child. There should only be one id, so using
  // the zero indice, *should* work.
  EXPECT_EQ(good, root->get_data()->ids[0]);
}
TEST(Quadtree_Tests, SetObjectWorks)
{
  using pong::Quadtree;
  Quadtree q({{0, 0}, 1000, 1000}, 1);

  // Insert some ball somewhere.
  q.makeBall({{50,50}, 50, 50});

  // Insert a ball somewhere.
  using pong::id_type;
  id_type ball = q.makeBall({{750,750}, 50, 50});

  // Move that ball somewhere relevant. A place that will cause some
  // side-effect.
  using pong::Object; using pong::Volume; using pong::PhysicsType;
  q.setObject(ball, Object{{{450,450}, 20, 20}, PhysicsType::Ball});

  // We should have a split.
  ASSERT_EQ(4, q.root()->children().size());

  const Quadtree::node_type* top_left_node = q.root()->children()[0];
  // But also another split
  EXPECT_EQ(4, top_left_node->children().size());

  // We should also find the `ball` object in the first child's root.
  const std::vector<id_type>& ids =
                                 top_left_node->children()[3]->get_data()->ids;

  using std::begin; using std::end;
  EXPECT_NE(end(ids), std::find(begin(ids), end(ids), ball));
}
TEST(Quadtree_Tests, FindContainingNodeWorks)
{
  using pong::Quadtree;
  Quadtree q({{0, 0}, 1000, 1000}, 1);

  // In between children 0 and 1.
  using pong::id_type;
  q.makeBall({{950, 950}, 50, 50});
  id_type id = q.makeBall({{475, 0}, 50, 50});
  ASSERT_TRUE(id);

  std::vector<const Quadtree::node_type*> expected{q.root()->children()[0],
                                                   q.root()->children()[1]};

  const auto& nodes =
              find_containing_nodes(const_cast<Quadtree::node_type*>(q.root()),
                                    id);

  using std::begin; using std::end;
  std::vector<const Quadtree::node_type*> actual(begin(nodes), end(nodes));

  EXPECT_EQ(expected, actual);
}
