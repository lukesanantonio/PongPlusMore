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
namespace pong
{
  // The purpose of the wrapper structure and functions is to allow for the
  // situation where Ok_T and Err_T are either the same type or are similar
  // enough to cause a template ambiguity.

  template <class T>
  struct Ok_Wrap
  {
    T obj;
  };

  template <class T>
  struct Err_Wrap
  {
    T obj;
  };

  // Stores the type information
  template <class T>
  Ok_Wrap<T> make_ok_result(T const& t) noexcept
  {
    return {t};
  }
  template <class T>
  Err_Wrap<T> make_err_result(T const& t) noexcept
  {
    return {t};
  }

  template <class Ok_T, class Err_T>
  struct Result
  {
    enum res_t {
      Ok, Err
    } type;

    boost::variant<Ok_T, Err_T> res;

    Result(Ok_T const& t) noexcept : type(Ok), res(t) {}
    Result(Err_T const& t) noexcept : type(Err), res(t) {}

    template <class T>
    Result(Ok_Wrap<T> const& t) noexcept : type(Ok), res(t.obj) {}

    template <class T>
    Result(Err_Wrap<T> const& t) noexcept : type(Err), res(t.obj) {}

    optional<Ok_T> ok() const noexcept;
    optional<Err_T> err() const noexcept;
  };

  template <class Ok_T, class Err_T>
  optional<Ok_T> Result<Ok_T, Err_T>::ok() const noexcept
  {
    if(type == Ok)
    {
      return boost::get<Ok_T>(res);
    }
    return boost::none;
  }
  template <class Ok_T, class Err_T>
  optional<Err_T> Result<Ok_T, Err_T>::err() const noexcept
  {
    if(type == Err)
    {
      return boost::get<Err_T>(res);
    }
    return boost::none;
  }
}
