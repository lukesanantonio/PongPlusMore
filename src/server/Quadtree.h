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
#pragma once
#include <memory>
#include <array>
#include <iterator>
#include <vector>
#include "common/util.h"
#include "ObjectManager.h"
#include "Node.hpp"
namespace pong
{
  struct Node_Content
  {
    Node_Content(ObjectManager* const objs,
                 int max_objs,
                 const Volume& v = Volume(),
                 const std::vector<id_type>& ids = std::vector<id_type>())
                 : objs(objs), max_objs(max_objs), v(v), ids(ids) {}
    ObjectManager *const objs;
    Volume v;
    int max_objs;
    std::vector<id_type> ids;
  };
  struct Quadtree
  {
    using node_type = Node<Node_Content>;

    Quadtree(const Volume& v, int max_objs = 5)
             : root_(std::make_unique<Node_Content>(&objs_, max_objs, v)) {}

    id_type insert(const Object& obj) noexcept;

    inline id_type makePaddle(Volume vol) noexcept
    { return insert(Object{vol, PhysicsType::Paddle}); }

    inline id_type makeBall(Volume vol) noexcept
    { return insert(Object{vol, PhysicsType::Ball}); }

    inline id_type insertObject(Volume vol, PhysicsOptions opt) noexcept
    { return insert(Object{vol, opt}); }

    void erase(id_type id);

    inline const node_type* root() const noexcept
    { return &this->root_; }
    inline const ObjectManager& obj_manager() const noexcept
    { return this->objs_; }

  private:
    ObjectManager objs_;
    node_type root_;
  };

}
