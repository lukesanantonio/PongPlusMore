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

#include <cstdint>

namespace pong
{
  using req_id = uint8_t;

  /*
   * Request Concept:
   *
   * nonstatic
   * - req_id id: Id of this particular request. Should be zero if no request
   * is needed or required.
   * - params_type params: Tuple of provided params (see params_type below).
   *
   * static:
   * - constexpr static char const* const methodname: name of the method field
   * that should trigger a parse of this type of request.
   *
   * types:
   * - params_type: A type alias to a tuple of a list of parseable types. When
   * this request type is triggered with the methodname static member, these
   * types will be parsed from the params field of the json request object.
   */
}

// Easy macro for defining classes that follow the Request concept.
#define REQUEST(classname, mname, ...)\
  struct classname\
  {\
    classname(req_id id = 0) noexcept : id(id) {}\
\
    template <class... ParamArgs>\
    classname(req_id id, ParamArgs&&... args) noexcept : id(id),\
              params(std::forward<ParamArgs>(args)...) {}\
\
    constexpr static char const* const methodname = mname;\
    using params_type = std::tuple<__VA_ARGS__>;\
\
    req_id id;\
    params_type params;\
  }

#include "../common/template_utility.hpp"

// Template utility code relating to requests.
namespace pong
{
  template <class T, class Enable = void>
  struct is_request : std::false_type {};

  template <class T>
  struct is_request<T, typename enable_if_type<decltype(T::methodname)>::type>
   : std::true_type {};
}

#include <boost/variant.hpp>
#include <tuple>
#include "json/json.h"

#include "../common/pif/helper.h"

#include <string>
#include "../common/vector.h"
#include "../io/Logger.h"

namespace pong
{
  /*
   * Request sent to the server when there is a parse error.
   * p1: Requested method name.
   */
  REQUEST(BadReq, "", std::string);

  /*
   * p1: Window Title
   * p2: Window size
   */
  REQUEST(NewWindowReq, "Core.NewWindow", std::string, math::vector<int>);

  /*
   * Starts a new gamestate and connects the given process to control.
   */
  REQUEST(AttachStateReq, "Core.AttachState");

  /*
   * Logs a message.
   * p1: severity
   * p2: message
   */
  REQUEST(LogReq, "Core.Log", Severity, std::string);

  /*!
   * \brief Parses for a pack of types each following the Request concept.
   */
  template <class... Reqs>
  struct Request_Parser
  {
    static boost::variant<Reqs...> parse(Json::Value const&) noexcept;
  };

  template <int N, class Req>
  std::enable_if_t<N == std::tuple_size<typename Req::params_type>::value>
  set_params(Req& r, Json::Value const& json) noexcept {}

  template <int N, class Req>
  std::enable_if_t<N < std::tuple_size<typename Req::params_type>::value>
  set_params(Req& r, Json::Value const& json) noexcept
  {
    // Parse the current param
    using tuple_t = typename Req::params_type;
    using formatter_t = FORMATTER_TYPE(std::tuple_element_t<N, tuple_t>);
    std::get<N>(r.params) = formatter_t::parse(json[N]);

    // Parse the next param
    set_params<N + 1, Req>(r, json);
  }

  template <int N, class... Reqs>
  std::enable_if_t<N == sizeof...(Reqs), boost::variant<Reqs...> >
  try_parse(Json::Value const& json) noexcept
  {
    // If the given methodname is *not* found tell the reciever of these
    // requests that an invalid method name was found. They'll handle the
    // response.
    return BadReq(0, json["method"].asString());
  }

  template <int N, class... Reqs>
  std::enable_if_t<N < sizeof...(Reqs), boost::variant<Reqs...> >
  try_parse(Json::Value const& json) noexcept
  {
    using req_type = pack_element_t<N, Reqs...>;
    if(req_type::methodname == json["method"].asString())
    {
      req_type r;

      // Set the id.
      r.id = json["id"].asInt();

      // Populate the tuple using the provided params.
      set_params<0, req_type>(r, json);

      return r;
    }
    else return try_parse<N+1, Reqs...>;
  }

  template <class... Reqs>
  boost::variant<Reqs...>
  Request_Parser<Reqs...>::parse(Json::Value const& json) noexcept
  {
    return try_parse<0, Reqs...>(json);
  }
}

BEGIN_FORMATTER_SCOPE
{
  template <class... Reqs>
  struct find_formatter<boost::variant<Reqs...>,
                       typename std::enable_if_t<
                       pong::all_of<pong::is_request<Reqs>::value...>::value,
                       boost::variant<Reqs...> >::type >
  {
    using type = pong::Request_Parser<Reqs...>;
  };
}
END_FORMATTER_SCOPE

#undef REQUEST
