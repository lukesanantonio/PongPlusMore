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
#include <queue>
namespace pong
{
  template <typename id_type>
  struct IDManager
  {
    using queue_type = std::queue<id_type>;

    id_type count = 0;
    queue_type removed_id_queue;

    inline id_type get() noexcept;
    inline void remove(id_type id) noexcept;
  };

  /*!
   * \brief Returns some valid which can be used on a new Object.
   *
   * \returns 0 if there are no ids available.
   */
  template <typename id_type>
  id_type IDManager<id_type>::get() noexcept
  {
    if(!removed_id_queue.empty())
    {
      id_type id = 0;
      id = removed_id_queue.front();
      removed_id_queue.pop();
      return id;
    }

    if(++this->count == 0)
    {
      // If we subtract one from it, this will continue forever.
      --this->count;
      return 0;
    }

    return this->count;
  }

  template <typename id_type>
  void IDManager<id_type>::remove(id_type id) noexcept
  {
    this->removed_id_queue.push(id);
  }

}
