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
#include <tuple>
#include "json/json.h"
#include "../common/template_utility.hpp"
namespace pong
{
  struct Invalid_Params_Exception {};

  struct Req_Method_Base
  {
    virtual void call(Json::Value const&) const = 0;
  };

  template <class> struct Req_Method;

  template <class Ret, class... Params>
  struct Req_Method<Ret(Params...)> : public Req_Method_Base
  {
    using function_t = std::function<Ret(Params...)>;

    Req_Method(function_t func) noexcept : f_(func) {}
    void call(Json::Value const&) const override;
  private:
    function_t f_;
  };

  template <class Ret, class... Params>
  void Req_Method<Ret(Params...)>::call(Json::Value const& json) const
  {
    // Parse the parameters.
    using tuple_t = std::tuple<Params...>;

    std::tuple<Params...> params;
    try
    {
      params = FORMATTER_TYPE(tuple_t)::parse(json);
    }
    catch(std::runtime_error& e)
    {
      throw Invalid_Params_Exception{};
    }

    call(f_, params);
  }
}
