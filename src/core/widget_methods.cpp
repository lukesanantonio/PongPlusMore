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

namespace engine
{
  template <class Member, class Object, class Fn>
  void add_set_method(std::string const& method,
                      pong::Req_Dispatcher& dispatch,
                      pong::ID_Map<Object>& map,
                      Fn const& fn)
  {
    dispatch.add_method<pong::id_type, Member>(method,
    [&map, &fn](pong::id_type id, Member const& t) -> pong::response_result
    {
      try
      {
        fn(map.find(id), t);
      }
      catch(std::out_of_range& e)
      {
        return pong::Error_Response{4, "Invalid id"};
      }
      return Json::Value(true);
    });
  }
  template <class Member, class Object, class Fn>
  void add_get_method(std::string const& method,
                      pong::Req_Dispatcher& dispatch,
                      pong::ID_Map<Object> const& map,
                      Fn const& fn)
  {
    dispatch.add_method<pong::id_type>(method,
    [&map, &fn](pong::id_type id) -> pong::response_result
    {
      try
      {
        auto const& value = fn(map.find(id));
        return FORMATTER_TYPE(Member)::dump(value);
      }
      catch(std::out_of_range& e)
      {
        return pong::Error_Response{4, "Invalid id"};
      }
    });
  }

  void add_label_methods(pong::Req_Dispatcher& dispatch, State& state,
                         pong::Logger& log)
  {
    using pong::id_type;
    namespace math = pong::math;

    dispatch.add_method<>("Label.Create_Label",
    [&state]() -> pong::response_result
    {
      pong::id_type id = state.labels_.insert(pong::Label<std::string>{});
      return Json::Value(id);
    });

    add_set_method<std::string>("Label.Set_String", dispatch, state.labels_,
                   [](auto& l, std::string const& str){ l.data(str); });
    add_get_method<std::string>("Label.Get_String", dispatch, state.labels_,
                   [](auto& l){ return l.data(); });

    add_set_method<math::vector<double> >("Label.Set_Position",
                                          dispatch, state.labels_,
    [](auto& l, math::vector<double> const& pos)
    { l.position(pos); });

    add_get_method<math::vector<double> >("Label.Get_Position", dispatch,
                                          state.labels_,
    [](auto& l) { return l.position(); });

    add_set_method<int>("Label.Set_Text_Height", dispatch, state.labels_,
                  [](auto& l, int size){ l.text_height(size); });
    add_get_method<int>("Label.Get_Text_Height", dispatch, state.labels_,
                  [](auto& l){ return l.text_height(); });
  }

  void add_widget_methods(pong::Req_Dispatcher& dispatch, State& state,
                          pong::Logger& log)
  {
    add_label_methods(dispatch, state, log);
  }
}
