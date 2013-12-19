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
 * \file Label.cpp
 * \brief Contains definitions for the Label class.
 */
#include "Label.h"
#include "common/crash.hpp"
namespace pong
{
  /*!
   * \brief Initializes a label with everything necessary to start rendering.
   *
   * \param text The text to display. Passed to pong::render_text.
   * \param text_height The height of the text in pixels. Passed to
   * pong::render_text.
   * \param pos The position of the top left corner where the surface will be
   * blitted.
   * \param font_renderer The font renderer implementation to use when
   * rasterizing the text!
   *
   * \note The text will be rendered monochrome in white. The background will
   * be solid black.
   */
  Label::Label(const std::string& text,
               int16_t text_height,
               math::vector<int16_t> pos,
               FontRenderer* font_renderer) noexcept :
               text_(text),
               text_height_(text_height),
               pos_(pos),
               font_renderer_(font_renderer)
  {
    //Set defaults.
    //Text color default: white
    this->text_color_.r = 0xff;
    this->text_color_.g = 0xff;
    this->text_color_.b = 0xff;

    //Background color default: black
    this->back_color_.r = 0x00;
    this->back_color_.g = 0x00;
    this->back_color_.b = 0x00;
  }

  /*!
   * \brief Initializes a label with everything + a custom text and background
   * color.
   *
   * \param text The text of the label.
   * \param text_height The pixel size of the text.
   * \param pos The position of the label when blitting.
   * \see Label::render(SDL_Surface*)
   * \param text_color The color of the text.
   * \param back_color The color of the empty space (if any) in the generated
   * SDL_Surface*.
   * \param font_renderer The existing FontRenderer implementation, will
   * not be deleted at any time!
   */
  Label::Label(const std::string& text,
               int16_t text_height,
               math::vector<int16_t> pos,
               SDL_Color text_color,
               SDL_Color back_color,
               FontRenderer* font_renderer) noexcept :
               text_(text),
               text_height_(text_height),
               pos_(pos),
               text_color_(text_color),
               back_color_(back_color),
               font_renderer_(font_renderer){}

  /*!
   * \brief Copy constructor.
   *
   * Does not copy the cache.
   */
  Label::Label(const Label& label) noexcept :
               Surface_Cache(),
               text_(label.text_),
               text_height_(label.text_height_),
               pos_(label.pos_),
               text_color_(label.text_color_),
               back_color_(label.back_color_){}
  /*!
   * \brief Move constructor.
   *
   * Moves the cache from the object passed in, notably.
   */
  Label::Label(Label&& label) noexcept :
               Surface_Cache(std::move(label)),
               text_(std::move(label.text_)),
               text_height_(label.text_height_),
               pos_(label.pos_),
               text_color_(label.text_color_),
               back_color_(label.back_color_){}

  /*!
   * \brief Copy assignment operator.
   *
   * Does not copy the cache, but invalidates it, just in case.
   */
  Label& Label::operator=(const Label& label) noexcept
  {
    this->text(label.text_);
    this->text_height(label.text_height_);

    this->position(label.pos_);

    this->text_color(label.text_color_);
    this->back_color(label.back_color_);

    return *this;
  }

  /*!
   * \brief Move assignment operator.
   *
   * Moves the cache from the other object to `this` one, notably.
   */
  Label& Label::operator=(Label&& label) noexcept
  {
    Surface_Cache::operator=(std::move(label));

    this->text(std::move(label.text_));
    this->text_height(label.text_height_);

    this->position(label.pos_);

    this->text_color(label.text_color_);
    this->back_color(label.back_color_);

    return *this;
  }

  /*!
   * \brief Renders the label using the passed in renderer.
   */
  void Label::render(SDL_Renderer* renderer) const
  {
    SDL_Texture* texture=SDL_CreateTextureFromSurface(renderer, this->cache());
    if(!texture) crash("Failed to create texture from Label surface!");

    SDL_Rect dest;
    dest.x = this->pos_.x;
    dest.y = this->pos_.y;
    dest.w = this->getSurfaceWidth();
    dest.h = this->getSurfaceHeight();
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_DestroyTexture(texture);
  }

  Label::ptr_type Label::generateCache_private() const
  {
    if(!this->font_renderer_)
      crash("Label: \"" + this->text_ + "\""
            " doesn't have a FontRenderer implementation!");


    //render the text with the specified colors.
    return this->font_renderer_->render_text(this->text_,
                                             this->text_height_,
                                             this->text_color_,
                                             this->back_color_);
  }
};
