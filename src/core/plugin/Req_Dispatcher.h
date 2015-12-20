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
#include <tuple>
#include <boost/optional.hpp>
#include "req.h"
#include "../common/result.h"
#include "../common/template_utility.hpp"

#include <functional>
namespace pong
{
  using boost::optional;
  struct Run_Context
  {
    Run_Context() noexcept;

    void set_ret(Params&& p) noexcept;
    void set_err(Params&& p) noexcept;

    bool is_ret() const noexcept;
    bool is_err() const noexcept;
    inline Params& params() noexcept { return params_; }

  private:
    enum ret_t
    {
      Ret, Err
    } type;
    Params params_;
  };

  struct Req_Method_Base
  {
    virtual void call(Run_Context& r, optional<Params> const&) const = 0;
  };

  template <class... Params>
  using method_t = std::function<Request(Params...)>;

  template <class... P_T>
  struct Req_Method : public Req_Method_Base
  {
    using fn_t = std::function<void(P_T...)>;

    Req_Method(fn_t func) noexcept : f_(func) {}

    /*!
     * \brief Calls the given function by converting the msgpack 'params'
     * object to arguments for the function.
     */
    void call(Run_Context& r, optional<Params> const&) const override;
  private:
    fn_t f_;
  };

  template <class... P_T>
  void Req_Method<P_T...>::call(Run_Context& r,
                                optional<Params> const& params) const
  {
    // Convert parameters

    auto vals = params->object.as<std::tuple<P_T...> >();

    // Handle failure with the run context
    // Pass parameters to function

    call(f_, vals);
  }

  struct Req_Dispatcher
  {
    template <class... Params, class F>
    void push_method(F f) noexcept;

    inline pong::Request dispatch(Request const& req);
  private:
    std::vector<std::unique_ptr<Req_Method_Base> > methods_;
  };

  template <class... Params, class F>
  void Req_Dispatcher::push_method(F f) noexcept
  {
    methods_.push_back(std::make_unique<Req_Method<Params...> >(f));
  }

  inline pong::Request Req_Dispatcher::dispatch(Request const& req)
  {
    // TODO: Handle invalid method

    Run_Context run;
    methods_[req.fn]->call(run, req.params);

    // For now generate the response no matter what.
    // Check the run context
    // No result
    if(run.is_ret())
    {
      Request ret;
      ret.fn = 0;
      ret.id = req.id;
      ret.params = std::move(run.params());
      return ret;
    }
    else
    {
      Request ret;
      ret.fn = 1;
      ret.id = req.id;
      ret.params = std::move(run.params());
      return ret;
    }
  }
}
