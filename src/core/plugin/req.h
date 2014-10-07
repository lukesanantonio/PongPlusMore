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
   * - params_t params: Tuple of provided params (see params_t below).
   *
   * static:
   * - constexpr static char const* const methodname: name of the method field
   * that should trigger a parse of this type of request.
   *
   * types:
   * - params_t: A type alias to a tuple of a list of parseable types. When
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
    using params_t = std::tuple<__VA_ARGS__>;\
\
    req_id id;\
    params_t params;\
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
   * p1: Params array.
   */
  REQUEST(Bad_Req, "", std::string, Json::Value);

  /*!
   * Requests engine exit.
   */
  REQUEST(Exit_Req, "Core.Exit");

  /*
   * p1: Window Title
   * p2: Window size
   */
  REQUEST(New_Window_Req, "Core.New_Window", std::string, math::vector<int>);

  /*
   * Starts a new gamestate and connects the given process to control.
   */
  REQUEST(Attach_State_Req, "Core.Attach_State");

  /*
   * Logs a message.
   * p1: severity
   * p2: message
   */
  REQUEST(Log_Req, "Core.Log", Severity, std::string);

  using Request = boost::variant<Bad_Req, Exit_Req, New_Window_Req,
                                 Attach_State_Req, Log_Req>;

  /*!
   * \brief Parses for a pack of types each following the Request concept.
   */
  template <class... Reqs>
  struct Request_Parser
  {
    static boost::variant<Reqs...> parse(Json::Value const&) noexcept;
    static Json::Value dump(boost::variant<Reqs...> const&) noexcept;
  };

  // Parsing code
  template <int N, class Req>
  std::enable_if_t<N == std::tuple_size<typename Req::params_t>::value>
  set_params(Req& r, Json::Value const& json) noexcept {}

  template <int N, class Req>
  std::enable_if_t<N < std::tuple_size<typename Req::params_t>::value>
  set_params(Req& r, Json::Value const& json) noexcept
  {
    // Parse the current param
    using tuple_t = typename Req::params_t;
    using formatter_t = FORMATTER_TYPE(std::tuple_element_t<N, tuple_t>);
    std::get<N>(r.params) = formatter_t::parse(json[N]);

    // Parse the next param
    set_params<N + 1, Req>(r, json);
  }

  template <int N, class... Reqs>
  std::enable_if_t<N == sizeof...(Reqs) ||
                   !is_request<pack_element_t<N, Reqs...> >::value,
                   boost::variant<Reqs...> >
  try_parse(Json::Value const& json) noexcept
  {
    // If the given methodname is *not* found tell the reciever of these
    // requests that an invalid method name was found. They'll handle the
    // response.
    return Bad_Req(0, json["method"].asString(), json["params"]);
  }

  template <int N, class... Reqs>
  std::enable_if_t<N < sizeof...(Reqs) &&
                   is_request<pack_element_t<N, Reqs...> >::value,
                   boost::variant<Reqs...> >
  try_parse(Json::Value const& json) noexcept
  {
    using req_type = pack_element_t<N, Reqs...>;
    if(req_type::methodname == json["method"].asString())
    {
      req_type r;

      // Set the id.
      r.id = json["id"].asInt();

      // Populate the tuple using the provided params.
      set_params<0, req_type>(r, json["params"]);

      return r;
    }
    else return try_parse<N+1, Reqs...>(json);
  }

  template <class... Reqs>
  boost::variant<Reqs...>
  Request_Parser<Reqs...>::parse(Json::Value const& json) noexcept
  {
    return try_parse<0, Reqs...>(json);
  }

  // Dumping code.
  struct Dump_Json_Visitor : public boost::static_visitor<Json::Value>
  {
    template <class Req_Type>
    Json::Value operator()(Req_Type const& req) const noexcept
    {
      Json::Value val;

      val["method"] = Req_Type::methodname;
      val["id"] = req.id;

      using params_formatter_t = FORMATTER_TYPE(typename Req_Type::params_t);
      val["params"] = params_formatter_t::dump(req.params);

      return val;
    }
  };

  template <class... Reqs>
  Json::Value
  Request_Parser<Reqs...>::dump(boost::variant<Reqs...> const& var) noexcept
  {
    return boost::apply_visitor(Dump_Json_Visitor(), var);
  }
}

BEGIN_FORMATTER_SCOPE
{
  template <class... Reqs>
  struct find_formatter<boost::variant<Reqs...>,
                       std::enable_if_t<
                        std::is_same<pong::Request,
                                     boost::variant<Reqs...> >::value > >
  {
    using type = pong::Request_Parser<Reqs...>;
  };
  template <class Req>
  struct find_formatter<Req, std::enable_if_t<pong::is_request<Req>::value> >
  {
    // For this specialization, parsing isn't supporting, but dumping straight
    // request objects will work fine.
    using type = pong::Request_Parser<Req>;
  };
}
END_FORMATTER_SCOPE

#undef REQUEST
