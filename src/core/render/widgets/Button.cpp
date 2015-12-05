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
/*!
 * \file Button.cpp
 * \brief Contains definitions for the Button class.
 */
#include "Button.h"
#include "../../common/center.hpp"
namespace pong
{
  /*!
   * \brief Initializes the button.
   *
   * \param text The text to display centered in the button.
   * \param vol The volume of the button. Position, Width, and Height.
   * \param enabled Whether or not the button should be clickable.
   */
  Button::Button(const std::string& text,
                 Volume<int> vol,
                 bool enabled,
                 text::Face* face,
                 text::Rasterizer* rasterizer,
                 SDL_Color text_color,
                 SDL_Color back_color,
                 SDL_Color disabled_color) :
                 label_(text, 0, {0,0}, text_color, face, rasterizer)
  {
    // Use the function so that the extra logic is done and so we don't have
    // to repeat it here.
    this->volume(vol);
    this->enabled(enabled);
    this->background_color(back_color);
    this->disabled_color(disabled_color);
  }

  Button::Button(const Button& b) noexcept
                 : label_(b.label_),
                   hotkeys_(b.hotkeys_),
                   vol_(b.vol_),
                   background_color_(b.background_color_),
                   disabled_color_(b.disabled_color_),
                   enabled_(b.enabled_),
                   on_click_(b.on_click_) {}

  Button::Button(Button&& b) noexcept
                 : label_(std::move(b.label_)),
                   hotkeys_(std::move(b.hotkeys_)),
                   vol_(std::move(b.vol_)),
                   background_color_(b.background_color_),
                   disabled_color_(b.disabled_color_),
                   enabled_(b.enabled_),
                   on_click_(std::move(b.on_click_)) {}

  Button& Button::operator=(const Button& b) noexcept
  {
    this->label_ = b.label_;
    this->hotkeys_ = b.hotkeys_;
    this->vol_ = b.vol_;

    this->background_color_ = b.background_color_;
    this->disabled_color_ = b.disabled_color_;
    this->enabled_ = b.enabled_;

    this->on_click_ = b.on_click_;

    return *this;
  }
  Button& Button::operator=(Button&& b) noexcept
  {
    this->label_ = std::move(b.label_);
    this->hotkeys_ = std::move(b.hotkeys_);
    this->vol_ = std::move(b.vol_);

    this->background_color_ = b.background_color_;
    this->disabled_color_ = b.disabled_color_;
    this->enabled_ = b.enabled_;

    this->on_click_ = std::move(b.on_click_);

    return *this;
  }

  void Button::add_hotkey(SDL_Scancode s) noexcept
  {
    this->hotkeys_.push_back(s);
  }
  void Button::remove_hotkey(SDL_Scancode s) noexcept
  {
    using std::begin; using std::end;
    auto new_end = std::remove(begin(hotkeys_), end(hotkeys_), s);
    this->hotkeys_.erase(new_end, end(hotkeys_));
  }

  /*!
   * \brief Renders the button with the label using the rendering context
   * passed in.
   *
   * Also renders the text centered on the button, if the button is too small
   * there may be visual artifacts.
   */
  void Button::render(SDL_Renderer* renderer) const
  {
    // Choose button color.
    const SDL_Color* color;
    if(this->enabled_ && this->on_click_)
    {
      color = &this->background_color_;
    }
    else color = &this->disabled_color_;

    // Get our previous draw color, and "push" it, sort of speak.
    SDL_Color prev_color;
    if(SDL_GetRenderDrawColor(renderer, &prev_color.r,
                                        &prev_color.g,
                                        &prev_color.b,
                                        &prev_color.a))
    {
      prev_color.r = 0xff;
      prev_color.g = 0xff;
      prev_color.b = 0xff;
      prev_color.a = 0xff;
    }

    // Calculate bounds.
    SDL_Rect button_rect;
    button_rect.x = vol_.pos.x;
    button_rect.y = vol_.pos.y;
    button_rect.w = vol_.width;
    button_rect.h = vol_.height;

    SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
    SDL_RenderFillRect(renderer, &button_rect);

    // Return to our old draw color, or "pop."
    SDL_SetRenderDrawColor(renderer, prev_color.r,
                                     prev_color.g,
                                     prev_color.b,
                                     prev_color.a);

    // Find out where to render the label!
    Vec<int> label_pos;
    label_pos.x = center(this->vol_.pos.x, this->vol_.width,
                         this->label_.surface_width());
    label_pos.y = center(this->vol_.pos.y, this->vol_.height,
                         this->label_.surface_height());
    this->label_.position(label_pos);

    this->label_.render(renderer);
  }

  /*!
   * \brief If the event passed in represents a mouse click event, it is
   * checked for possibly clicking on the button.
   *
   * If there is indeed a mouse click inside the bounds of the current button,
   * Button::on_click_ is called!
   * \see Button::onClick
   */
  void Button::handleEvent(const SDL_Event& event)
  {
    if(!this->enabled_) return;
    if(event.type == SDL_MOUSEBUTTONUP)
    {
      if(event.button.button == SDL_BUTTON_LEFT)
      {
        Vec<int> point;
        point.x = event.button.x;
        point.y = event.button.y;

        if(is_in(vol_.pos.x, vol_.pos.x + vol_.width - 1, point.x) &&
           is_in(vol_.pos.y, vol_.pos.y + vol_.height - 1, point.y))
        {
          if(on_click_) on_click_();
        }
      }
    }
    if(event.type == SDL_KEYDOWN && !this->hotkeys_.empty())
    {
      SDL_Scancode pressed = event.key.keysym.scancode;
      using std::begin; using std::end;
      if(std::find(begin(hotkeys_), end(hotkeys_), pressed) != end(hotkeys_))
      {
        this->on_click_();
      }
    }
  }

};
