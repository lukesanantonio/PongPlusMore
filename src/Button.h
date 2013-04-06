/*!
 * \file Button.h
 * \brief Contains declarations for the Button class.
 */
#ifndef ULTIMATE_PONG_BUTTON_H
#define ULTIMATE_PONG_BUTTON_H
#include <string>
#include <SDL/SDL.h>
#include "Label.h"
#include "vector.hpp"
namespace pong
{
  class Button
  {
  public:
    explicit Button(const std::string& text = "",
                    math::vector pos = math::vector(),
                    std::size_t width = 0,
                    std::size_t height = 0);
    ~Button() noexcept;
    Button(const Button&) = delete;
    Button(Button&&) = delete;
    Button& operator=(const Button&) = delete;
    Button& operator=(Button&&) = delete;

    /*!
     * \brief Renders everything onto the surface so that the top left corner
     * of the button is at Button::pos_
     */
    void render(SDL_Surface* surface) const;

    /*!
     * \brief Generates Button::image_, an all-white surface of width and height
     * of Button::width_ and Button::height_ respectively.
     *
     * This function is here to allow for the client to specify generation when
     * it's a good time, or not, this function is required, since it will be
     * called when necessary.
     *
     * \post Button::image_is_out_of_date_ == false
     * \post Button::image_->w == Button::width_
     * \post Button::image_->h == Button::height_
     *
     * \throws std::runtime_error if SDL fails to create the surface for any
     * reason.
     */
    void generateImage() const;
  private:
    mutable Label label_;
    math::vector pos_;
    std::size_t width_;
    std::size_t height_;

    mutable SDL_Surface* image_ = nullptr;
    /*!
     * \brief Whether or not the all-white image used as the button itself needs
     * to be regenerated.
     *
     * \sa Button::image_
     */
    mutable bool image_is_out_of_date_ = true;
  };
};
#endif
