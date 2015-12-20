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
#include "Button.h"
#include <unordered_map>
#include <string>
#include <functional>
namespace ug
{
  template <class ValueType>
  struct ButtonSwitch
  {
    using callback_type = std::function<void(ValueType)>;

    ButtonSwitch(Button& b, std::string const& prefix) noexcept
                 : b_(b), prefix_text_(prefix) {}

    inline void add_switch(std::string const& str, ValueType v) noexcept;
    inline void on_change(callback_type cb);

    void operator()() noexcept;

  private:
    inline void set_button_string_() noexcept;

    Button& b_;
    std::string prefix_text_;
    std::unordered_map<std::string, ValueType> switches_;
    std::string cur_;
    callback_type on_change_;
  };
  template <class ValueType>
  inline void ButtonSwitch<ValueType>::
  add_switch(std::string const& str, ValueType v) noexcept
  {
    switches_.emplace(str, v);
    if(switches_.size() == 1)
    {
      this->cur_ = str;
      this->set_button_string_();
    }
  }
  template <class ValueType>
  inline void ButtonSwitch<ValueType>::on_change(callback_type cb)
  {
    this->on_change_ = cb;
  }

  template <class ValueType>
  inline void ButtonSwitch<ValueType>::set_button_string_() noexcept
  {
    this->b_.text(this->prefix_text_ + this->cur_);
    this->on_change_(this->switches_[this->cur_]);
  }

  template <class ValueType>
  void ButtonSwitch<ValueType>::operator()() noexcept
  {
    if(this->switches_.empty()) return;

    auto where = this->switches_.find(this->cur_);
    ++where;
    if(where == this->switches_.end())
    {
      where = this->switches_.begin();
    }

    this->cur_ = where->first;

    this->set_button_string_();
  }
}
