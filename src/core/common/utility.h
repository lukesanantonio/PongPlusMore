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
#include <type_traits>
#include <functional>
#include <vector>
#include <string>
namespace pong
{
  namespace detail
  {
    template <bool is_const, typename Dest, typename Source,
              typename F>
    std::vector<Dest> vector_cast(std::conditional_t<is_const,
                                                   const std::vector<Source>&,
                                                   std::vector<Source>&> v,
                                  F f)
    {
      std::vector<Dest> d;

      using Source_Reference =
                          std::conditional_t<is_const, const Source&, Source&>;

      for(Source_Reference s : v)
      {
        d.push_back(f(s));
      }

      return d;
    }
  }

  template <typename Dest, typename Source, typename F>
  inline std::vector<Dest> vector_cast(const std::vector<Source>& v, F f)
  {
    return detail::vector_cast<true, Dest, Source>(v, f);
  }
  template <typename Dest, typename Source>
  inline std::vector<Dest> vector_cast(const std::vector<Source>& v)
  {
    return detail::vector_cast<true, Dest, Source>(v,
    [](const auto& c) { return c; });
  }
  template <typename Dest, typename Source, typename F>
  inline std::vector<Dest> vector_cast(std::vector<Source>& v, F f)
  {
    return detail::vector_cast<false, Dest, Source>(v, f);
  }
  template <typename Dest, typename Source>
  inline std::vector<Dest> vector_cast(std::vector<Source>& v)
  {
    return detail::vector_cast<false, Dest, Source>(v,
    [](auto& c) { return c; });
  }

  /*!
   * \brief Converts a vector of some smart pointer to a vector of just those
   * pointers.
   */
  template <typename Dest, class SP>
  std::vector<Dest> get_data_vector(const std::vector<SP>& v) noexcept
  {
    return vector_cast<Dest>(v, [](const SP& p) { return p.get(); });
  }

  inline std::vector<char> vec_from_string(const std::string& s) noexcept
  {
    return std::vector<char>(s.begin(), s.end());
  }
}
