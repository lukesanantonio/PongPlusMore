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
namespace pong
{
  /*!
   * \brief Checks whether the specified point is in the button.
   */
  static bool checkClick(Button* button, math::vector<int16_t> point)
  {
    //The button can only be clicked if it is enabled.
    if(button->enabled())
    {
      //Cache the parameters...
      math::vector<int16_t> pos = button->position();
      int16_t width = button->width();
      int16_t height = button->height();

      //Check to see if the button occupies the point... Sooo:
      if(point.x <= pos.x + width  && point.x >= pos.x &&
         point.y <= pos.y + height && point.y >= pos.y)
      {
        return true;
      }
    }
    return false;
  }

  /*!
   * \brief Initializes the button.
   *
   * \param text The text to display centered in the button.
   * \param pos The position of the top left corner of the button in SDL
   * window space.
   * \param width The width of the button.
   * \param height The height of the button.
   * \param enabled Whether or not the button should be clickable.
   * \param font_renderer Font Renderer implementation passed directly to
   * the internal label: Button::label_
   */
  Button::Button(const std::string& text,
                 math::vector<int16_t> pos,
                 int16_t width,
                 int16_t height,
                 bool enabled,
                 FontRenderer* font_renderer) :
                 label_(text, 24, {0,0}, font_renderer),
                 pos_(pos),
                 width_(width),
                 height_(height),
                 enabled_(enabled)
  {
    //Set the initial colors of the label.
    SDL_Color black;
    black.r = 0x00;
    black.g = 0x00;
    black.b = 0x00;
    this->label_.text_color(black);

    SDL_Color white;
    white.r = 0xff;
    white.g = 0xff;
    white.b = 0xff;
    this->label_.back_color(white);
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
    button_rect.x = this->pos_.x;
    button_rect.y = this->pos_.y;
    button_rect.w = this->width_;
    button_rect.h = this->height_;

    //Choose button color.
    SDL_Color color;
    if(this->enabled_)
    {
      color.r = 0xff;
      color.g = 0xff;
      color.b = 0xff;
    }
    else
    {
      color.r = 0x55;
      color.g = 0x55;
      color.b = 0x55;
    }
    color.a = 0xff;

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

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &button_rect);

    //Return to our old draw color.
    SDL_SetRenderDrawColor(renderer, prev_color.r,
                                     prev_color.g,
                                     prev_color.b,
                                     prev_color.a);

    //Find where to render the label!
    math::vector<int16_t> label_pos;
    label_pos.x = center(this->pos_.x, this->width_,
                         this->label_.getSurfaceWidth());
    label_pos.y = center(this->pos_.y, this->height_,
                         this->label_.getSurfaceHeight());
    this->label_.position(label_pos);

    //Render the label.
    this->label_.render(renderer);
  }

  /*!
   * \brief Add a functor to the list of functions which need to be called
   * if the button should be clicked on.
   *
   * \returns The connection returned by boost::signals2::signal::connect().
   * \sa Button::on_click_
   */
  boost::signals2::connection Button::executeOnClick(
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
   * \see Button::executeOnClick
   */
  void Button::handleEvent(const SDL_Event& event)
  {
    if(event.type == SDL_MOUSEBUTTONUP)
    {
      if(event.button.button == SDL_BUTTON_LEFT)
      {
        math::vector<int16_t> point;
        point.x = event.button.x;
        point.y = event.button.y;

        if(checkClick(this, point))
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
    SDL_Color back_color;
    if(this->enabled_)
    {
      back_color.r = 0xff;
      back_color.g = 0xff;
      back_color.b = 0xff;
    }
    else
    {
      back_color.r = 0x55;
      back_color.g = 0x55;
      back_color.b = 0x55;
    }
    this->label_.back_color(back_color);
  }
};
