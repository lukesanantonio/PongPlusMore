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
/*!
 * \file Button.cpp
 * \brief Contains definitions for the Button class.
 */
#include "Button.h"
#include "common/center.hpp"
#include "server/collision_util.h"
namespace pong
{
  /*!
   * \brief Initializes the button.
   *
   * \param text The text to display centered in the button.
   * \param vol The volume of the button. Position, Width, and Height.
   * \param enabled Whether or not the button should be clickable.
   * \param font_renderer Font Renderer implementation passed directly to
   * the internal label: Button::label_
   */
  Button::Button(const std::string& text,
                 Volume vol,
                 bool enabled,
                 FontRenderer* font_renderer) :
                 label_(text, 24, {0,0}, font_renderer),
                 vol_(vol)
  {
    // Set the initial colors of the label.
    this->label_.text_color({0xff, 0xff, 0xff, 0xff});

    // Use the function so that the label's background color is set properly.
    this->enabled(enabled);
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
    //Calculate bounds.
    SDL_Rect button_rect;
    button_rect.x = vol_.pos.x;
    button_rect.y = vol_.pos.y;
    button_rect.w = vol_.width;
    button_rect.h = vol_.height;

    //Choose button color.
    const SDL_Color* color;
    if(this->enabled_) color = &this->background_color_;
    else color = &this->disabled_color_;

    //Get our previous draw color.
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

    SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
    SDL_RenderFillRect(renderer, &button_rect);

    //Return to our old draw color.
    SDL_SetRenderDrawColor(renderer, prev_color.r,
                                     prev_color.g,
                                     prev_color.b,
                                     prev_color.a);

    //Find where to render the label!
    math::vector<int> label_pos;
    label_pos.x = center(this->vol_.pos.x, this->vol_.width,
                         this->label_.getSurfaceWidth());
    label_pos.y = center(this->vol_.pos.y, this->vol_.height,
                         this->label_.getSurfaceHeight());
    this->label_.position(label_pos);

    // Render the label. Its background color should be consistent with the
    // one we chose earlier because of the enabled function.
    this->label_.render(renderer);
  }

  /*!
   * \brief Add a functor to the list of functions which need to be called
   * if the button should be clicked on.
   *
   * \returns The connection returned by boost::signals2::signal::connect().
   * \sa Button::on_click_
   */
  boost::signals2::connection Button::onClick(
                       const boost::signals2::signal<void ()>::slot_type& slot)
  {
    return this->on_click_.connect(slot);
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
    if(event.type == SDL_MOUSEBUTTONUP)
    {
      if(event.button.button == SDL_BUTTON_LEFT)
      {
        math::vector<int> point;
        point.x = event.button.x;
        point.y = event.button.y;

        if(isIn(vol_.pos.x, vol_.pos.x + vol_.width - 1, point.x) &&
           isIn(vol_.pos.y, vol_.pos.y + vol_.height - 1, point.y))
        {
          this->on_click_();
        }
      }
    }
  }

  /*!
   * \brief Sets whether the button can be clicked.
   */
  void Button::enabled(bool enabled) noexcept
  {
    this->enabled_ = enabled;

    //Make sure to set the background color in the Label so it is consistent
    //with the background of the button.
    if(this->enabled_)
    {
      this->label_.back_color(this->background_color_);
    }
    else
    {
      this->label_.back_color(this->disabled_color_);
    }
  }
};
