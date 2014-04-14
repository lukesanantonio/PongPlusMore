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
#include "collision_util.h"
namespace pong
{
  struct Node_Content
  {
    Node_Content(ObjectManager* const objs,
                 int max_objs,
                 int level,
                 int max_level,
                 const Volume& v = Volume(),
                 const std::vector<id_type>& ids = std::vector<id_type>())
                 : objs(objs), max_objs(max_objs), max_level(max_level),
                   current_level(level), v(v), ids(ids) {}
    ObjectManager *const objs;
    Volume v;
    int max_objs;
    int current_level;
    int max_level;
    std::vector<id_type> ids;
  };
  struct Quadtree
  {
    using node_type = Node<Node_Content>;

    Quadtree(const Volume& v, int max_objs = 5, int max_level = 5)
             : root_(std::make_unique<Node_Content>(&objs_, max_objs,
                                                    1, max_level,  v)) {}

    const Object& findObject(id_type) const;
    void setObject(id_type, const Object&);

    id_type insert(const Object& obj) noexcept;

    inline id_type makePaddle(Volume vol) noexcept
    { return insert(Object{vol, PhysicsType::Paddle}); }

    inline id_type makeBall(Volume vol) noexcept
    { return insert(Object{vol, PhysicsType::Ball}); }

    inline id_type insertObject(Volume vol, PhysicsOptions opt) noexcept
    { return insert(Object{vol, opt}); }

    ObjectManager::iterator erase(ObjectManager::const_iterator pos);
    ObjectManager::iterator erase(ObjectManager::const_iterator pos,
                                  ObjectManager::const_iterator last);
    ObjectManager::size_type erase(id_type id);

    inline const node_type* root() const noexcept
    { return &this->root_; }
    inline const ObjectManager& obj_manager() const noexcept
    { return this->objs_; }

  private:
    ObjectManager objs_;
    node_type root_;
  };

  template <typename NT, class ND>
  std::vector<Node<NT, ND>*> find_containing_nodes(Node<NT, ND>* root,
                                                   id_type id) noexcept
  {

    if(!root) return {nullptr};
    if(!root->get_data()) return {nullptr};

    const Object& obj = root->get_data()->objs->findObject(id);

    if(isIntersecting(root->get_data()->v, obj.getVolume()))
    {
      // If leaf:
      if(root->children().empty()) return {root};

      // If parent:
      std::vector<Node<NT, ND>*> nodes;
      for(auto& child : root->children())
      {
        auto new_nodes = find_containing_nodes(child, id);
        using std::begin; using std::end;
        nodes.insert(end(nodes), begin(new_nodes), end(new_nodes));
      }
      return nodes;
    }

    return {};
  }
}
