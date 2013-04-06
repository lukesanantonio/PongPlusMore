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
  /*!
   * \brief Class wrapping clickable button.
   */
  class Button
  {
  public:
    /*!
     * \brief Initializes the button.
     *
     * \param text The text to display centered in the button.
     * \param pos The position of the top left corner of the button in SDL
     * window space.
     * \param width The width of the button.
     * \param height The height of the button.
     */
    explicit Button(const std::string& text = "",
                    math::vector pos = math::vector(),
                    std::size_t width = 0,
                    std::size_t height = 0);
    /*!
     * \brief Uninitializes the image if that hasn't been done already.
     */
    ~Button() noexcept;

    /*!
     * \brief Deleted copy constructor.
     */
    Button(const Button&) = delete;
    /*!
     * \brief Deleted move constructor.
     */
    Button(Button&&) = delete;
    /*!
     * \brief Deleted copy assignment operator.
     */
    Button& operator=(const Button&) = delete;
    /*!
     * \brief Deleted move assignment operator.
     */
    Button& operator=(Button&&) = delete;

    /*!
     * \brief Renders everything onto the surface so that the top left corner
     * of the button is at Button::pos_
     *
     * Also renders the text centered on the button, if the button is too small
     * there will be massive visual artifacts.
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
     * reason. The what message will explain where the error occured.
     */
    void generateImage() const;
  private:
    /*!
     * \brief The internal label used to render the text onto the button.
     *
     * Mutable so that the render function can modify it's position. Every
     * other field should not be modified without the client's knowledge,
     * though.
     */
    mutable Label label_;

    /*!
     * \brief The position of the top left corner of the button in SDL window
     * space.
     */
    math::vector pos_;
    /*!
     * \brief The width of the button.
     */
    std::size_t width_;
    /*!
     * \brief The height of the button.
     */
    std::size_t height_;

    /*!
     * \brief The cached image for the button. Regenerated when necessary, that
     * is usually when the width or height of the button changes.
     */
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
