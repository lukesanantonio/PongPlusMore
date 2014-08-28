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
#include "ObjectManager.h"
#include <algorithm>
#include "Server.h"
namespace pong
{
  ObjectManager::ObjectManager(const Server& s) noexcept
  {
    if(s.objects().empty()) return;
    for(id_type id : s.objects())
    {
      objs_.emplace(id, s.find_object(id));
    }

    // Make id_counter_ the max, then add the missing ones in between to
    // the removed_id_queue_.
    using std::begin; using std::end;

    // We don't have to worry about *this being empty, since we already checked
    // for the emptiness of the server.
    id_type max_id = std::get<0>(*std::max_element(begin(*this), end(*this),
    [](const auto& p1, const auto& p2)
    {
      return p1.first < p2.first;
    }));
    this->id_counter_.count = max_id;

    for(id_type i = 1; i <= max_id; ++i)
    {
      // If this particular id is *not* found: It means we can use it.
      if(this->objs_.find(i) == end(this->objs_))
      {
        this->id_counter_.removed_id_queue.push(i);
      }
    }
  }
}
