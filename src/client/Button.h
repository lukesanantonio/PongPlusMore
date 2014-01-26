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
 * \file Button.h
 * \brief Contains declarations for the Button class.
 */
#pragma once
#include <string>
#include <boost/signals2.hpp>
#include "SDL.h"
#include "Label.h"
#include "render_text.h"
#include "common/vector.h"
#include "common/Volume.h"
namespace pong
{
  /*!
   * \brief Class wrapping clickable button.
   */
  class Button
  {
  public:
    using signal_t = boost::signals2::signal<void ()>;
  public:
    explicit Button(const std::string& text = "",
                    Volume vol = {{0,0},300,100},
                    bool enabled = true,
                    FontRenderer* font_renderer = nullptr);

    ~Button() noexcept = default;

    Button(const Button&) = delete;
    Button(Button&&) = delete;
    Button& operator=(const Button&) = delete;
    Button& operator=(Button&&) = delete;

    void render(SDL_Renderer*) const;

    boost::signals2::connection onClick(const signal_t::slot_type& slot);

    void handleEvent(const SDL_Event&);

    inline void text(const std::string& text);
    inline std::string text() const;

    inline void volume(const Volume& vol) noexcept;
    inline Volume volume() const noexcept;

    inline void text_color(SDL_Color col) noexcept;
    inline SDL_Color text_color() const noexcept;

    inline void background_color(SDL_Color col) noexcept;
    inline SDL_Color background_color() const noexcept;

    inline void disabled_color(SDL_Color col) noexcept;
    inline SDL_Color disabled_color() const noexcept;

    void enabled(bool enabled) noexcept;
    inline bool enabled() const noexcept;

    inline void font_renderer(FontRenderer* font_renderer) noexcept;
    inline FontRenderer* font_renderer() const noexcept;
  private:
    /*!
     * \brief The internal label used to render the text onto the button.
     *
     * \note Marked mutable so we can change it's rendering position when
     * rendering (const function).
     */
    mutable Label label_;

    /*!
     * \brief The volume of the Button.
     */
    Volume vol_;

    SDL_Color background_color_;
    SDL_Color disabled_color_;

    /*!
     * \brief Whether or not the button can be clicked.
     *
     * \note Also, the button is rendered grayed out if this is false.
     */
    bool enabled_;

    /*!
     * \brief This signal is emitted when the button is clicked on.
     *
     * \sa Button::onClick()
     */
    signal_t on_click_;
};

  /*!
   * \brief Sets the text to be displayed on the button.
   */
  inline void Button::text(const std::string& text)
  {
    this->label_.text(text);
  }
  /*!
   * \brief Returns the text displayed on the button as a string.
   *
   * \returns Button::label_.text()
   */
  inline std::string Button::text() const
  {
    return this->label_.text();
  }

  /*!
   * \brief Sets the Volume of the Button.
   */
  inline void Button::volume(const Volume& vol) noexcept
  {
    this->vol_ = vol;
  }
  inline Volume Button::volume() const noexcept
  {
    return this->vol_;
  }

  /*!
   * \brief Sets the text color of the button.
   */
  inline void Button::text_color(SDL_Color col) noexcept
  {
    this->label_.text_color(col);
  }
  /*!
   * \brief Returns the text color of the button.
   */
  inline SDL_Color Button::text_color() const noexcept
  {
    return this->label_.text_color();
  }

  /*!
   * \brief Sets the background color of the button when enabled.
   */
  inline void Button::background_color(SDL_Color col) noexcept
  {
    this->background_color_ = col;
  }
  /*!
   * \brief Returns the background color of the button when enabled.
   */
  inline SDL_Color Button::background_color() const noexcept
  {
    return this->background_color_;
  }

  /*!
   * \brief Sets the color of the background of the button when it is
   * disabled.
   */
  inline void Button::disabled_color(SDL_Color col) noexcept
  {
    this->disabled_color_ = col;
  }
  /*!
   * \brief Returns the color of the background of the button when it is
   * disabled.
   */
  inline SDL_Color Button::disabled_color() const noexcept
  {
    return this->disabled_color_;
  }

  /*!
   * \brief Returns whether or not the button is enabled or not.
   *
   * \returns Button::enabled_
   */
  inline bool Button::enabled() const noexcept
  {
    return this->enabled_;
  }

  /*!
   * \brief Sets the font renderer in the internal label, discarding the one
   * already in there.
   *
   * \note The Font Renderer implementations need to be managed from the
   * outside.
   */
  inline void Button::font_renderer(FontRenderer* font_renderer) noexcept
  {
    this->label_.font_renderer(font_renderer);
  }
  /*!
   * \brief Returns a pointer to the current font renderer active in the
   * internal label.
   */
  inline FontRenderer* Button::font_renderer() const noexcept
  {
    return this->label_.font_renderer();
  }
};
