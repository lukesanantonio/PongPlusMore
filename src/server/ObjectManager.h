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
#include <json/json.h>
#include "Object.h"

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

    ObjectManager() noexcept {}
    ObjectManager(const Server& s) noexcept;

    inline id_type makePaddle(Volume vol) noexcept;
    inline id_type makeBall(Volume vol) noexcept;
    inline id_type insertObject(Volume vol, PhysicsOptions opt) noexcept;

    inline const Object& findObject(id_type) const;
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
  private:
    map_type objs_;
    id_type id_counter_ = 0;
    std::queue<id_type> removed_id_queue_;

    inline id_type getNextID() noexcept;
  };

  Json::Value dumpJSON(const ObjectManager& objs) noexcept;

  /*!
   * \brief Returns some valid which can be used on a new Object.
   *
   * \returns 0 if there are no ids available.
   */
  id_type ObjectManager::getNextID() noexcept
  {
    if(!removed_id_queue_.empty())
    {
      id_type id = 0;
      id = removed_id_queue_.front();
      removed_id_queue_.pop();
      return id;
    }

    if(++id_counter_ == 0)
    {
      // If we subtract one from it, this will continue forever.
      --id_counter_;
      return 0;
    }

    return id_counter_;
  }

  /*!
   * \brief Adds a new Object to the pool with 'Paddle' rules. \sa PhysicsType
   *
   * \returns A zero id when insertion failed, otherwise the id of the newly
   * inserted object.
   */
  id_type ObjectManager::makePaddle(Volume vol) noexcept
  {
    id_type id = getNextID();
    if(!id) return 0;

    this->objs_.emplace(id, Object{vol, PhysicsType::Paddle});
    return id;
  }

  /*!
   * \brief Adds a new Object to the pool with 'Ball' rules. \sa PhysicsType
   *
   * \returns A zero id when insertion failed, otherwise the id of the newly
   * inserted object.
   */
  id_type ObjectManager::makeBall(Volume vol) noexcept
  {
    id_type id = getNextID();
    if(!id) return 0;

    this->objs_.emplace(id, Object{vol, PhysicsType::Ball});
    return id;
  }

  inline id_type ObjectManager::insertObject(Volume vol,
                                             PhysicsOptions opt) noexcept
  {
    id_type id = getNextID();
    if(!id) return 0;

    this->objs_.emplace(id, Object{vol, opt});
    return id;
  }

  inline auto ObjectManager::erase(const_iterator pos) -> iterator
  {
    using std::end;
    if(pos != end(this->objs_)) this->removed_id_queue_.push(pos->first);
    return this->objs_.erase(pos);
  }
  inline auto ObjectManager::erase(const_iterator first,
                                   const_iterator last) -> iterator
  {
    const_iterator orig_first = first;
    for(; first != last; ++first) this->removed_id_queue_.push(first->first);
    return this->objs_.erase(orig_first, last);
  }
  inline auto ObjectManager::erase(key_type id) -> size_type
  {
    using std::end;
    if(this->objs_.find(id) != end(this->objs_))
      this->removed_id_queue_.push(id);

    return this->objs_.erase(id);
  }

  /*!
   * \brief Returns a const reference to the object with the passed in id.
   * \throws std::out_of_range if the id doesn't correlate with any Object.
   */
  const Object& ObjectManager::findObject(id_type id) const
  {
    return objs_.at(id);
  }
  void ObjectManager::setObject(id_type id, const Object& obj)
  {
    this->objs_.at(id) = obj;
  }

  /*!
   * \brief Returns whether or not the id is of an object that is a paddle.
   */
  inline bool isPaddle(const ObjectManager& objs, id_type id)
  {
    return isPaddle(objs.findObject(id));
  }
  /*!
   * \brief Returns whether or not the id is of an object that is a ball.
   */
  inline bool isBall(const ObjectManager& objs, id_type id)
  {
    return isBall(objs.findObject(id));
  }
}
