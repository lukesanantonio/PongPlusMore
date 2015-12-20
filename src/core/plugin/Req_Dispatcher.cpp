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
#include "Req_Dispatcher.h"
namespace ug
{
  Run_Context::Run_Context() noexcept : type(Ret)
  {
    params_.zone = msgpack::zone();
    params_.object = msgpack::object(std::vector<msgpack::object>(),
                                     params_.zone);
  }
  void Run_Context::set_ret(Params&& p) noexcept
  {
    type = Ret;
    params_ = std::move(p);
  }
  void Run_Context::set_err(Params&& p) noexcept
  {
    type = Err;
    params_ = std::move(p);
  }

  bool Run_Context::is_ret() const noexcept
  {
    return type == Ret;
  }
  bool Run_Context::is_err() const noexcept
  {
    return type == Err;
  }
}
