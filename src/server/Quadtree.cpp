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
#include "Quadtree.h"
#include "collision_util.h"
#include <numeric>
namespace pong
{
  bool insert(Node<Node_Content>& root, id_type id) noexcept
  {
    // Logic error!
    if(!root.get_data()) return false;

    if(isIntersecting(root.get_data()->v,
                      root.get_data()->objs->findObject(id).getVolume()))
    {
      auto& max_objs = root.get_data()->max_objs;

      // Leaf
      if(root.children().empty())
      {
        // We have room?!
        if(root.get_data()->ids.size() + 1 <= max_objs ||
           root.get_data()->current_level + 1 > root.get_data()->max_level)
        {
          root.get_data()->ids.push_back(id);
          return true;
        }
        else
        {
          // Split, let the next if handle it.
          auto quads = find_volume_quads(root.get_data()->v);
          auto& objs = root.get_data()->objs;

          root.push_child(std::make_unique<Node_Content>(
                                            objs, max_objs,
                                            root.get_data()->current_level + 1,
                                            root.get_data()->max_level,
                                            quads[0]));
          root.push_child(std::make_unique<Node_Content>(
                                            objs, max_objs,
                                            root.get_data()->current_level + 1,
                                            root.get_data()->max_level,
                                            quads[1]));
          root.push_child(std::make_unique<Node_Content>(
                                            objs, max_objs,
                                            root.get_data()->current_level + 1,
                                            root.get_data()->max_level,
                                            quads[2]));
          root.push_child(std::make_unique<Node_Content>(
                                            objs, max_objs,
                                            root.get_data()->current_level + 1,
                                            root.get_data()->max_level,
                                            quads[3]));

          std::vector<id_type> ids = root.get_data()->ids;
          root.get_data()->ids.clear();
          for(id_type id : ids) { insert(root, id); }
        }
      }
      // Parent
      if(!root.children().empty())
      {
        bool has_been_added = false;
        //                      .- This fails because of different locations
        //                         of the points.
        //std::for_each(root.children().begin(), root.children().end(),

        auto children = root.children();
        for(Node<Node_Content>* n : children)
        {
          has_been_added = insert(*n, id) || has_been_added;
        };
        return has_been_added;
      }
    }
    // If the Node container and the object aren't intersecting than what
    // is the point.
    /* else */ return false;
  }

  bool remove(Node<Node_Content>& root, id_type id)
  {
    // Logic error!
    if(!root.get_data()) return false;

    using std::begin; using std::end;

    // Leaf
    if(root.children().empty())
    {
      auto& ids = root.get_data()->ids;

      auto iter = std::find(begin(ids), end(ids), id);
      if(iter == end(ids))
      {
        return false;
      }

      root.get_data()->ids.erase(iter);
      return true;
    }

    // else Parent:

    // Remove the id.
    {
      bool has_been_removed = false;
      for(auto& child : root.children())
      {
        has_been_removed = remove(*child, id) || has_been_removed;
      }
      if(!has_been_removed) return false;
    }

    // We removed *something*.

    // Important: Use all the Leaf nodes children of the root. Not necessarily
    // the direct children.
    int total_children = std::accumulate(begin(root), end(root), 0,
    [](int i, Node<Node_Content>& n)
    {
      return i + n.get_data()->ids.size();
    });

    if(total_children <= root.get_data()->max_objs)
    {
      // We, the root, can become a leaf again!
      auto& ids = root.get_data()->ids;

      // For every leaf child in root.
      for(auto& child : root)
      {
        // Go through the ids of every child.
        auto& child_ids = child.get_data()->ids;
        for(id_type id : child_ids)
        {
          // Add them to our big list of ids.
          ids.push_back(id);
        }
      }

      // Now remove all children.
      for(auto& child : root)
      {
        // We are too far in now. If this fails we can't go back right now.
        root.remove_child(&child);
      }
    }
    return true;
  }

  // Member implementations.

  id_type Quadtree::insert(const Object& obj) noexcept
  {
    // Add to the object manager.
    id_type id = this->objs_.insert(obj);
    if(!id) return 0;

    using pong::insert;
    if(!insert(this->root_, id))
    {
      this->objs_.erase(id);
      return 0;
    }

    return id;
  }
  ObjectManager::iterator Quadtree::erase(ObjectManager::const_iterator pos)
  {
    // Remove the object from the tree.
    remove(this->root_, pos->first);

    // Remove the object from the object manager.
    auto ret_iter = this->objs_.erase(pos);
    return ret_iter;
  }
  ObjectManager::iterator Quadtree::erase(ObjectManager::const_iterator pos,
                                          ObjectManager::const_iterator last)
  {
    // Remove the objects from the tree.
    std::for_each(pos, last,
    [&](const auto& pair)
    {
      remove(this->root_, pair.first);
    });

    // Remove the objects from the object manager.
    return this->objs_.erase(pos, last);
  }
  ObjectManager::size_type Quadtree::erase(id_type id)
  {
    // Remove the object from the tree.
    remove(this->root_, id);

    // Remove the object from the object manager.
    return this->objs_.erase(id);
  }

  const Object& Quadtree::findObject(id_type id) const
  {
    // TODO: could this be more efficient searching for the id taking into
    // account its volume?
    return this->objs_.findObject(id);
  }
  void Quadtree::setObject(id_type id, const Object& obj)
  {
    // Is it out of the quadtree? Can we just ignore it.
    if(!isIntersecting(this->findObject(id).getVolume(),
                       this->root_.get_data()->v))
    {
      //Don't bother doing anything with it.
      return;
    }

    // Don't remove it from the object manager, just the quadtree.
    remove(this->root_, id);

    // Now change it in the object manager.
    this->objs_.setObject(id, obj);

    // Now add it back to our quadtree.
    using pong::insert;
    insert(this->root_, id);
  }
}
