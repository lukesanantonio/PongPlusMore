/*
 * uGlue - Glue many languages together into a whole with ukernel-inspired RPC.
 * Copyright (C) 2015 Luke San Antonio
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
#include <string>
#include <type_traits>
#include <unordered_map>
#include "template_utility.hpp"
namespace ug
{ struct Lang
  {
    std::unordered_map<std::string, std::string> dict;
  };

  void set_lang(Lang const& lang) noexcept;
  Lang get_lang() noexcept;

  inline std::string to_string_helper(std::string str) noexcept { return str; }

  inline std::string to_string_helper(char const* const str) noexcept
  { return str; }
  inline std::string to_string_helper(char* str) noexcept
  { return str; }

  template <class T>
  std::string to_string_helper(T&& v) noexcept { return std::to_string(v); }

  inline std::string format(std::string s, int pos, std::string i) noexcept
  {
    // If we have no more to replace just append the original string to
    // whatever we have. This will leave extra percent signs if there are
    // less arguments than percent signs but that's intentional.
    return i + s.substr(pos);
  }

  template <class First, class... Args>
  std::string format(std::string str, int pos, std::string i,
                     First&& first, Args&&... args) noexcept
  {
    // If our position is past the end of the string, just return the
    // intermediate string since it should have been completed. The reason
    // we have to handle it here is because the user may provide extra
    // arguments making the template above this one never be instantiated.
    if(static_cast<int>(str.size()) <= pos) return format(str, pos, i);

    auto c = str[pos];
    if(c == '%')
    {
      // Append the first argument as a string instead of the percent sign.
      i += to_string_helper(std::forward<First>(first));

      // Format further without the 'first' argument since it has been
      // used up.
      return format(str, pos + 1, i, std::forward<Args>(args)...);
    }
    else
    {
      // Pass in the original string, the new intermediate, and the
      // arguments as is, since we haven't used them up yet.
      return format(str, pos + 1, i + c,
                    std::forward<First>(first), std::forward<Args>(args)...);
    }
  }

  template <class... Args>
  std::string format_str(std::string str, Args&&... args) noexcept
  {
    return format(str, 0, "", std::forward<Args>(args)...);
  }

  template <class... Args>
  void write_format_str(std::ostream& o, std::string str,
                        Args&&... args) noexcept
  {
    o << format_str(str, std::forward<Args>(args)...);
  }

  template <class... Args>
  std::string translate(std::string str, Args&&... args) noexcept
  {
    return format(get_lang().dict.at(str), 0, "", std::forward<Args>(args)...);
  }

  template <class... Args>
  std::string t(std::string str, Args&&... args) noexcept
  {
    return translate(str, std::forward<Args>(args)...);
  }
}
