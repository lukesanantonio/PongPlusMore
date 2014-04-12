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
 *
 * \file serialize.hpp
 * \brief All the functions relating to serialization of objects to JSON.
 */
#pragma once
#include <json/json.h>
#include "vector.h"
#include "server/Node.hpp"
#include "server/ObjectManager.h"
#include "server/Quadtree.h"
#include <boost/type_traits/is_fundamental.hpp>

namespace pong
{
  // Template forward-declarations.

  template <typename T,
            class = std::enable_if_t<boost::is_fundamental<T>::value, T> >
  inline Json::Value dumpJSON(const T& t) noexcept;

  template <class Iter1, class Iter2>
  Json::Value dumpJSON(Iter1 i1, Iter2 i2) noexcept;

  template <typename point_type>
  Json::Value dumpJSON(const math::vector<point_type>& v) noexcept;

  template <typename NT, class ND>
  Json::Value dumpJSON(const Node<NT, ND>& n) noexcept;

  // Templates

  // Primitive-Types cover-all.
  template <typename T, class>
  inline Json::Value dumpJSON(const T& t) noexcept
  {
    return Json::Value(t);
  }

  template <class Iter1, class Iter2>
  Json::Value dumpJSON(Iter1 i1, Iter2 i2) noexcept
  {
    Json::Value v(Json::arrayValue);
    for(; i1 != i2; ++i1)
    {
      v.append(dumpJSON(*i1));
    }
    return v;
  }

  template <typename point_type>
  Json::Value dumpJSON(const math::vector<point_type>& v) noexcept
  {
    Json::Value root(Json::objectValue);

    root["x"] = v.x;
    root["y"] = v.y;

    return root;
  }

  template <typename NT, class ND>
  Json::Value dumpJSON(const Node<NT, ND>& n) noexcept
  {
    Json::Value v(Json::objectValue);

    if(n.get_data())
    {
      v["Data"] = dumpJSON(*n.get_data().get());
    }

    for(const auto& c : n.children())
    {
      v["Children"].append(dumpJSON(*c));
    }
    return v;
  }

  // Concrete.

  struct Volume;
  Json::Value dumpJSON(const Volume&) noexcept;

  struct PhysicsOptions;
  Json::Value dumpJSON(const PhysicsOptions&) noexcept;

  struct Object;
  Json::Value dumpJSON(const Object&) noexcept;

  Json::Value dumpJSON(const ObjectManager& objs) noexcept;

  struct Server;
  inline Json::Value dumpJSON(const Server& s) noexcept
  {
    return dumpJSON(ObjectManager(s));
  }

  struct Node_Content;
  Json::Value dumpJSON(const Node_Content& q) noexcept;

  inline Json::Value dumpJSON(const Quadtree& q) noexcept
  {
    return dumpJSON(*q.root());
  }

}
