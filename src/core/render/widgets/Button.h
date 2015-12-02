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
 *
 * \file Button.h
 * \brief Contains declarations for the Button class.
 */
#pragma once
#include <string>
#include <functional>
#include "SDL.h"
#include "Label.h"
#include "../text.h"
#include "../../common/vector.h"
#include "../../common/volume.h"
namespace pong
{
  /*!
   * \brief Class wrapping a clickable button.
   */
  class Button
  {
  public:
    using click_cb = std::function<void ()>;

  public:
    explicit Button(const std::string& text = "",
                    Volume<int> vol = {{0,0},0,0},
                    bool enabled = true,
                    text::Face* face = nullptr,
                    text::Rasterizer* rasterizer = nullptr,
                    SDL_Color text_color = {0x00, 0x00, 0x00, 0xff},
                    SDL_Color back_color = {0xff, 0xff, 0xff, 0x00},
                    SDL_Color disabled_color = {0x77, 0x77, 0x77, 0xff});

    ~Button() noexcept = default;

    Button(const Button&) noexcept;
    Button(Button&&) noexcept;
    Button& operator=(const Button&) noexcept;
    Button& operator=(Button&&) noexcept;

    void render(SDL_Renderer*) const;

    void on_click(click_cb const& cb);

    void add_hotkey(SDL_Scancode s) noexcept;
    void remove_hotkey(SDL_Scancode s) noexcept;

    void handleEvent(const SDL_Event&);

    inline void text(const std::string& text);
    inline std::string text() const;

    inline void volume(const Volume<int>& vol) noexcept;
    inline Volume<int> volume() const noexcept;

    inline void text_color(SDL_Color col) noexcept;
    inline SDL_Color text_color() const noexcept;

    inline void background_color(SDL_Color col) noexcept;
    inline SDL_Color background_color() const noexcept;

    inline void disabled_color(SDL_Color col) noexcept;
    inline SDL_Color disabled_color() const noexcept;

    inline void enabled(bool enabled) noexcept;
    inline bool enabled() const noexcept;

    inline void font_face(text::Face* face) noexcept;
    inline text::Face* font_face() const noexcept;

    inline void rasterizer(text::Rasterizer* rasterizer) noexcept;
    inline text::Rasterizer* rasterizer() const noexcept;
  private:
    /*!
     * \brief The internal label used to render the text onto the button.
     *
     * \note Marked mutable so we can change it's rendering position when
     * rendering (const function).
     */
    mutable Label<std::string> label_;

    /*!
     * \brief A list of keys that activate the button.
     */
    std::vector<SDL_Scancode> hotkeys_;

    /*!
     * \brief The volume of the Button.
     */
    Volume<int> vol_;

    /*!
     * \brief The background color of the button when it is enabled.
     */
    SDL_Color background_color_;
    /*!
     * \brief The background color of the button when it is disabled.
     */
    SDL_Color disabled_color_;

    /*!
     * \brief Whether or not the button can be clicked.
     */
    bool enabled_;

    /*!
     * \brief This function is called when the button is clicked on.
     *
     * \sa Button::on_click()
     */
    click_cb on_click_;
};

  /*!
   * \brief Sets the text to be displayed on the button.
   */
  inline void Button::text(const std::string& text)
  {
    this->label_.data(text);
  }
  /*!
   * \brief Returns the text displayed on the button as a string.
   *
   * \returns Button::label_.data()
   */
  inline std::string Button::text() const
  {
    return this->label_.data();
  }

  /*!
   * \brief Sets the Volume of the Button.
   */
  inline void Button::volume(const Volume<int>& vol) noexcept
  {
    // The text height may need to be updated.
    this->label_.text_height(vol.height);

    this->vol_ = vol;
  }
  inline Volume<int> Button::volume() const noexcept
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
   * \brief Sets whether the button can be clicked.
   */
  inline void Button::enabled(bool enabled) noexcept
  {
    this->enabled_ = enabled;
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

  inline void Button::font_face(text::Face* face) noexcept
  {
    this->label_.font_face(face);
  }
  inline text::Face* Button::font_face() const noexcept
  {
    return this->label_.font_face();
  }

  inline void Button::rasterizer(text::Rasterizer* rasterizer) noexcept
  {
    this->label_.rasterizer(rasterizer);
  }
  inline text::Rasterizer* Button::rasterizer() const noexcept
  {
    return this->label_.rasterizer();
  }

  /*!
   * \brief Set the function to be called when the button is clicked.
   *
   * \sa Button::on_click_
   */
  inline void Button::on_click(click_cb const& cb)
  {
    this->on_click_ = cb;
  }
};
