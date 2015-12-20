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
#include "req.h"
namespace ug
{
  Params::Params() noexcept : object(zone) {}
  Params::Params(msgpack::object&& obj, msgpack::zone&& zone) noexcept
    : zone(std::move(zone)), object(std::move(obj)) {}

  Params::Params(Params const& p) noexcept : object(p.object, zone) {}
  Params::Params(Params&& p) noexcept :
    zone(std::move(p.zone)), object(std::move(p.object)) {}

  Params& Params::operator=(Params const& p) noexcept
  {
    this->object = msgpack::object(p.object, zone);
    return *this;
  }
  Params& Params::operator=(Params&& p) noexcept
  {
    this->zone = std::move(p.zone);
    this->object = std::move(p.object);
    return *this;
  }
}
