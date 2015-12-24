/*
 * uGlue - Glue many languages together into a whole with ukernel-inspired RPC.
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
#include "dispatch.h"
namespace ug
{
  void dispatch(std::vector<method_t>& methods, Request req,
                Run_Context* ctx) noexcept
  {
    if(req.fn >= methods.size())
    {
      // Get the hell outta here before we jump to mysterious code.
      return;
    }

    if(!req.params)
    {
      msgpack::object obj;
      obj.type = msgpack::type::ARRAY;
      obj.via.array.size = 0;
      obj.via.array.ptr = nullptr;

      req.params = Params{msgpack::clone(obj)};
    }
    (methods[req.fn])(ctx, *req.params);
  }
}
