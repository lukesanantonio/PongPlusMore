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
#include "response.h"

#include <functional>
namespace pong
{
  struct Invalid_Params_Exception {};

  struct Req_Method_Base
  {
    virtual std::string method() const noexcept = 0;
    virtual response_result call(Json::Value const&) const = 0;
  };

  template <class... Params>
  using method_t = std::function<response_result(Params...)>;

  template <class... Params>
  struct Req_Method : public Req_Method_Base
  {
    using function_t = method_t<Params...>;

    Req_Method(std::string const& method, function_t func) noexcept :
               method_(method), f_(func) {}

    /*!
     * \brief Returns the method that a request needs to call this function.
     */
    inline std::string method() const noexcept override { return method_; }

    /*!
     * \brief Calls the given function by converting the json 'params' object
     * to arguments for the function.

     * \throws Invalid_Params_Exception if parsing the parameters fails for
     * some reason.

     * Will rethrow any exception thrown resulting from the function call.
     */
    response_result call(Json::Value const&) const override;
  private:
    std::string method_;
    function_t f_;
  };

  template <class... Params>
  response_result Req_Method<Params...>::call(Json::Value const& json) const
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

    return pong::call(f_, params);
  }

  struct Req_Dispatcher
  {
    template <class... Params, class F>
    void add_method(std::string const& method, F f) noexcept;

    inline pong::Response dispatch(Request const& req);
  private:
    std::vector<std::unique_ptr<Req_Method_Base> > methods_;
  };

  template <class... Params, class F>
  void Req_Dispatcher::add_method(std::string const& method, F f) noexcept
  {
    methods_.push_back(std::make_unique<Req_Method<Params...> >(method, f));
  }

  inline pong::Response Req_Dispatcher::dispatch(Request const& req)
  {
    for(auto& ptr : methods_)
    {
      if(req.method == ptr->method())
      {
        response_result res;
        if(req.params) res = ptr->call(req.params.value());
        else res = ptr->call(Json::ValueType::arrayValue);

        return pong::Response{req.id, res};
      }
    }

    // Unknown method!
    return pong::Response{req.id, Error_Response{-32601, "Unknown method"} };
  }
}
