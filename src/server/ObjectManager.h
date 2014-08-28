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
#include <unordered_map>
#include <queue>
#include "common/IDManager.hpp"
#include "Object.h"
#include "common/Cache.h"

namespace pong
{
  struct Server;

  using id_type = uint16_t;
  using map_type = std::unordered_map<id_type, Object>;
  struct ObjectManager
  {
    using iterator = map_type::iterator;
    using const_iterator = map_type::const_iterator;
    using value_type = map_type::value_type;
    using key_type = map_type::key_type;
    using size_type = map_type::size_type;
    using id_type = pong::id_type;

    inline ObjectManager() noexcept
    {
      this->ids_cache_.gen_func([this](auto ptr)
      {
        auto ids = std::make_unique<std::vector<id_type> >();
        for(auto&& pair : *this)
        {
          ids->push_back(std::get<0>(pair));
        }
        return ids;
      });
    }
    ObjectManager(const Server& s) noexcept;

    inline id_type insert(const Object& obj) noexcept;

    inline const Object& find_object(id_type) const;
    inline void setObject(id_type, const Object&);

    inline iterator erase(const_iterator pos);
    inline iterator erase(const_iterator first, const_iterator last);
    inline size_type erase(key_type id);

    inline iterator begin() { return objs_.begin(); }
    inline const_iterator begin() const { return objs_.begin(); }
    inline const_iterator cbegin() const { return objs_.cbegin(); }

    inline iterator end() { return objs_.end(); }
    inline const_iterator end() const { return objs_.end(); }
    inline const_iterator cend() const { return objs_.cend(); }

    inline size_type size() const { return objs_.size(); }

    inline std::vector<id_type> ids() const noexcept;
  private:
    map_type objs_;
    IDManager<id_type> id_counter_;

    mutable Cache<std::vector<id_type> > ids_cache_;
  };

  inline id_type ObjectManager::insert(const Object& obj) noexcept
  {
    id_type id = this->id_counter_.get();
    if(!id) return 0;

    this->objs_.emplace(id, obj);
    this->ids_cache_.invalidate();
    return id;
  }

  inline auto ObjectManager::erase(const_iterator pos) -> iterator
  {
    using std::end;
    if(pos != end(this->objs_)) this->id_counter_.remove(pos->first);

    this->ids_cache_.invalidate();
    return this->objs_.erase(pos);
  }
  inline auto ObjectManager::erase(const_iterator first,
                                   const_iterator last) -> iterator
  {
    const_iterator orig_first = first;
    for(; first != last; ++first) this->id_counter_.remove(first->first);

    this->ids_cache_.invalidate();
    return this->objs_.erase(orig_first, last);
  }
  inline auto ObjectManager::erase(key_type id) -> size_type
  {
    using std::end;
    if(this->objs_.find(id) != end(this->objs_))
      this->id_counter_.remove(id);

    this->ids_cache_.invalidate();
    return this->objs_.erase(id);
  }

  /*!
   * \brief Returns a const reference to the object with the passed in id.
   * \throws std::out_of_range if the id doesn't correlate with any Object.
   */
  const Object& ObjectManager::find_object(id_type id) const
  {
    return objs_.at(id);
  }
  void ObjectManager::setObject(id_type id, const Object& obj)
  {
    this->objs_.at(id) = obj;
  }

  inline std::vector<id_type> ObjectManager::ids() const noexcept
  {
    return *this->ids_cache_.cache();
  }

  /*!
   * \brief Returns whether or not the id is of an object that is a paddle.
   */
  inline bool isPaddle(const ObjectManager& objs, id_type id)
  {
    return isPaddle(objs.find_object(id));
  }
  /*!
   * \brief Returns whether or not the id is of an object that is a ball.
   */
  inline bool isBall(const ObjectManager& objs, id_type id)
  {
    return isBall(objs.find_object(id));
  }
}
