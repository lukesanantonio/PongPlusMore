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
#include "common/volume.h"
#include <string>
#include "Label.h"
namespace pong
{
  struct Text_Box
  {
    Text_Box(Volume const& vol = Volume(), int text_size = 15,
             int blink_rate = 50, text::Face* face = nullptr,
             text::Rasterizer* = nullptr) noexcept;

    void handle_event(const SDL_Event& e) noexcept;
    void render(SDL_Renderer* r) const noexcept;

    inline Volume volume() const noexcept;
    inline void volume(Volume const&) noexcept;

    inline int text_size() const noexcept;
    inline void text_size(int) noexcept;

    inline int cur_pos() const noexcept;
    inline void cur_pos(int) noexcept;

    inline int blink_rate() const noexcept;
    inline void blink_rate(int) noexcept;

    inline std::string buffer() const noexcept;
    inline void buffer(std::string const&) noexcept;

    inline text::Face* font_face() const noexcept;
    inline void font_face(text::Face*) noexcept;

    inline text::Rasterizer* rasterizer() const noexcept;
    inline void rasterizer(text::Rasterizer*) noexcept;
  private:
    Volume vol_;
    int cur_pos_;
    int blink_rate_;
    mutable Label<std::string> label_;
  };

  inline Volume Text_Box::volume() const noexcept
  {
    return this->vol_;
  }
  inline void Text_Box::volume(Volume const& vol) noexcept
  {
    this->vol_ = vol;
  }

  inline int Text_Box::text_size() const noexcept
  {
    return this->label_.text_height();
  }
  inline void Text_Box::text_size(int size) noexcept
  {
    return this->label_.text_height(size);
  }

  inline int Text_Box::cur_pos() const noexcept
  {
    return this->cur_pos_;
  }
  inline void Text_Box::cur_pos(int pos) noexcept
  {
    this->cur_pos_ = pos;
  }

  inline int Text_Box::blink_rate() const noexcept
  {
    return this->blink_rate_;
  }
  inline void Text_Box::blink_rate(int rate) noexcept
  {
    this->blink_rate_ = rate;
  }

  inline std::string Text_Box::buffer() const noexcept
  {
    return this->label_.data();
  }
  inline void Text_Box::buffer(std::string const& buf) noexcept
  {
    this->label_.data(buf);
  }
  inline text::Face* Text_Box::font_face() const noexcept
  {
    return this->label_.font_face();
  }
  inline void Text_Box::font_face(text::Face* face) noexcept
  {
    this->label_.font_face(face);
  }
  inline text::Rasterizer* Text_Box::rasterizer() const noexcept
  {
    return this->label_.rasterizer();
  }
  inline void Text_Box::rasterizer(text::Rasterizer* raster) noexcept
  {
    return this->label_.rasterizer(raster);
  }
}
