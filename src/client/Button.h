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
namespace pong
{
  /*!
   * \brief Class wrapping clickable button.
   */
  class Button
  {
  public:
    explicit Button(const std::string& text = "",
                    math::vector<int16_t> pos = math::vector<int16_t>(),
                    int16_t width = 0,
                    int16_t height = 0,
                    bool enabled = true,
                    FontRenderer* font_renderer = nullptr);

    ~Button() noexcept = default;

    Button(const Button&) = delete;
    Button(Button&&) = delete;
    Button& operator=(const Button&) = delete;
    Button& operator=(Button&&) = delete;

    void render(SDL_Surface*) const;

    boost::signals2::connection executeOnClick(
                      const boost::signals2::signal<void ()>::slot_type& slot);

    void handleEvent(const SDL_Event&);

    inline void text(const std::string& text);
    inline std::string text() const;

    inline void position(const math::vector<int16_t> pos) noexcept;
    inline math::vector<int16_t> position() const noexcept;

    inline void width(int16_t width) noexcept;
    inline int16_t width() const noexcept;

    inline void height(int16_t height) noexcept;
    inline int16_t height() const noexcept;

    void enabled(bool enabled) noexcept;
    inline bool enabled() const noexcept;

    inline void font_renderer(FontRenderer* font_renderer) noexcept;
    inline FontRenderer* font_renderer() const noexcept;
  private:
    /*!
     * \brief The internal label used to render the text onto the button.
     */
    Label label_;

    /*!
     * \brief The position of the top left corner of the button in SDL window
     * space.
     */
    math::vector<int16_t> pos_;

    /*!
     * \brief The width of the button.
     */
    int16_t width_;

    /*!
     * \brief The height of the button.
     */
    int16_t height_;

    /*!
     * \brief Whether or not the button can be clicked.
     *
     * \note Also, the button is rendered grayed out if this is false.
     */
    bool enabled_;

    /*!
     * \brief This signal is emitted when the button is clicked on.
     *
     * \sa Button::executeOnClick()
     */
    boost::signals2::signal<void ()> on_click_;
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
   * \brief Sets the internal position of the button.
   */
  inline void Button::position(const math::vector<int16_t> pos) noexcept
  {
    this->pos_ = pos;
  }
  /*!
   * \brief Returns the internal position of the button.
   *
   * \returns Button::pos_
   */
  inline math::vector<int16_t> Button::position() const noexcept
  {
    return this->pos_;
  }

  /*!
   * \brief Sets the width of the button to render.
   */
  inline void Button::width(int16_t width) noexcept
  {
    this->width_ = width;
  }
  /*!
   * \brief Returns the width of the button.
   *
   * \returns Button::width_
   */
  inline int16_t Button::width() const noexcept
  {
    return this->width_;
  }

  /*!
   * \brief Sets the height of the button to render.
   */
  inline void Button::height(int16_t height) noexcept
  {
    this->height_ = height;
    //Set the text height relative to the button height.
    this->label_.text_height(this->height_ - 10);
  }
  /*!
   * \brief Returns the height of the button
   *
   * \returns Button::height_
   */
  inline int16_t Button::height() const noexcept
  {
    return this->height_;
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
