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
#include "buffer.h"
namespace ug
{
  bool operator==(std::vector<uchar> const& buf1,
                  std::vector<uchar> const& buf2) noexcept
  {
    if(buf1.size() != buf2.size()) return false;
    return std::equal(buf1.begin(), buf1.end(), buf2.end());
  }

  std::vector<uchar> buf_from_string(const std::string& s) noexcept
  {
    return std::vector<uchar>(s.begin(), s.end());
  }

  namespace literals
  {
    std::vector<uchar>
      operator "" _buf(char const* str, std::size_t size) noexcept
    {
      return std::vector<uchar>(str, str + size);
    }
  }
}

