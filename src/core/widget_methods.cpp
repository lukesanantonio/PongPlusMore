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
#include "engine.h"
#define CHECK_VALID_ID(map, id)\
try\
{\
  map.find(id);\
}\
catch(std::out_of_range& e)\
{\
  return Error_Response{4, "Invalid id"};\
}

namespace engine
{
  void add_label_methods(pong::Req_Dispatcher& dispatch, State& state,
                         pong::Logger& log)
  {
    using res_t = pong::response_result;
    using pong::id_type;

    namespace math = pong::math;

    using pong::Error_Response;

    dispatch.add_method<>("Label.Create_Label",
    [&state]() -> res_t
    {
      pong::id_type id = state.labels_.insert(pong::Label<std::string>{});
      return Json::Value(id);
    });
    // Error codes: 4 - Invalid id.
    dispatch.add_method<id_type, std::string>("Label.Set_String",
    [&state](id_type id, std::string const& str) -> res_t
    {
      CHECK_VALID_ID(state.labels_, id);
      state.labels_.find(id).data(str);
      return Json::Value(true);
    });
    dispatch.add_method<id_type>("Label.Get_String",
    [&state](id_type id) -> res_t
    {
      CHECK_VALID_ID(state.labels_, id);
      return state.labels_.find(id).data();
    });
    dispatch.add_method<id_type, math::vector<double> >("Label.Set_Position",
    [&state](id_type id, math::vector<double> const& pos) -> res_t
    {
      CHECK_VALID_ID(state.labels_, id);
      state.labels_.find(id).position(pos);
      return Json::Value(true);
    });
    dispatch.add_method<id_type>("Label.Get_Position",
    [&state](id_type id) -> res_t
    {
      CHECK_VALID_ID(state.labels_, id);
      auto const& pos = state.labels_.find(id).position();
      return FORMATTER_TYPE(math::vector<double>)::dump(pos);
    });
    dispatch.add_method<id_type>("Label.Get_Text_Height",
    [&state](id_type id ) -> res_t
    {
      CHECK_VALID_ID(state.labels_, id);
      return Json::Value(state.labels_.find(id).text_height());
    });
    dispatch.add_method<id_type, int>("Label.Set_Text_Height",
    [&state](id_type id, int size) -> res_t
    {
      CHECK_VALID_ID(state.labels_, id);
      state.labels_.find(id).text_height(size);
      return Json::Value(true);
    });
  }

  void add_widget_methods(pong::Req_Dispatcher& dispatch, State& state,
                          pong::Logger& log)
  {
    add_label_methods(dispatch, state, log);
  }
}
