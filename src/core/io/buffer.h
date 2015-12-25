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
#pragma once
#include <vector>
#include <string>
namespace ug
{
  using uchar = unsigned char;
  using buf_t = std::vector<uchar>;

  bool operator==(std::vector<uchar> const& b1,
                  std::vector<uchar> const& b2) noexcept;

  std::vector<uchar> buf_from_string(const std::string& s) noexcept;

  namespace literals
  {
    std::vector<uchar>
      operator "" _buf(char const* str, std::size_t size) noexcept;
  }
}
