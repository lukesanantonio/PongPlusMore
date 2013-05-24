/*!
 * \file Button.h
 * \brief Contains declarations for the Button class.
 */
#ifndef ULTIMATE_PONG_BUTTON_H
#define ULTIMATE_PONG_BUTTON_H
#include <string>
#include <boost/signals2.hpp>
#include <SDL/SDL.h>
#include "Label.h"
#include "vector.hpp"
#include "EventSignals.h"
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
     * \param text_height The size of the text, passed directly into the Label
     * member's constructor.
     */
    explicit Button(const std::string& text = "",
                    math::vector pos = math::vector(),
                    std::size_t width = 0,
                    std::size_t height = 0,
                    std::size_t text_height = 24);
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

    /*!
     * \brief Add a functor to the list of functions which need to be called
     * if the button should be clicked on.
     *
     * \returns The connection returned by boost::signals2::signal::connect().
     * \sa Button::checkClick()
     * \sa Button::on_click_
     */
    boost::signals2::connection executeOnClick(
                       const boost::signals2::signal<void ()>::slot_type& slot);

    /*!
     * \brief Adds necessary handlers to the event signals object so that when
     * the user clicks the mouse the button will check and possibly emit another
     * signal.
     */
    void useSignals(EventSignals& signals) const;

    /*!
     * \brief Sets the internal label of the button.
     *
     * \note Does not automatically tell the label to invert it's palette.
     * Either do it yourself or see no text! This is to avoid any possible
     * confusion.
     * \note Does not invalidate the cache.
     */
    inline void label(const Label& label);
    /*!
     * \brief Returns the internal label (a copy).
     *
     * \returns Button::label_
     */
    inline Label label() const;

    /*!
     * \brief Sets the text height of the label. Does invalidate the label
     * cache, in theory, but we don't have to worry about that.
     *
     * \note This is just a helper for getting the label, changing the text
     * size and setting it again with Button::label(const Label&).
     */
    inline void text_height(std::size_t text_height) noexcept;
    /*!
     * \brief Returns the text height of the label.
     *
     * \returns Button::label_::text_height()
     */
    inline std::size_t text_height() const noexcept;

    /*!
     * \brief Sets the internal position of the button.
     *
     * \note Does not invalidate the cache.
     */
    inline void position(const math::vector pos) noexcept;
    /*!
     * \brief Returns the internal position of the button.
     *
     * \returns Button::pos_
     */
    inline math::vector position() const noexcept;

    /*!
     * \brief Sets the width of the button to render.
     *
     * \note Does indeed invalidate the cache.
     */
    inline void width(std::size_t width) noexcept;
    /*!
     * \brief Returns the width of the button.
     *
     * \returns Button::width_
     */
    inline std::size_t width() const noexcept;

    /*!
     * \brief Sets the height of the button to render.
     *
     * \note Does indeed invalidate the cache.
     */
    inline void height(std::size_t height) noexcept;
    /*!
     * \brief Returns the height of the button
     *
     * \returns Button::height_
     */
    inline std::size_t height() const noexcept;
  private:
    /*!
     * \brief The internal label used to render the text onto the button.
     */
    Label label_;

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

    /*!
     * \brief This signal is emitted when the button is clicked on.
     *
     * \sa Button::executeOnClick()
     * \sa Button::checkClick()
     */
    boost::signals2::signal<void ()> on_click_;
  };

  inline void Button::label(const Label& label)
  {
    this->label_ = label;
  }
  inline Label Button::label() const
  {
    return this->label_;
  }

  inline void Button::text_height(std::size_t text_height) noexcept
  {
    this->label_.text_height(text_height);
  }
  inline std::size_t Button::text_height() const noexcept
  {
    return this->label_.text_height();
  }

  inline void Button::position(const math::vector pos) noexcept
  {
    this->pos_ = pos;
  }
  inline math::vector Button::position() const noexcept
  {
    return this->pos_;
  }

  inline void Button::width(std::size_t width) noexcept
  {
    this->width_ = width;
    this->image_is_out_of_date_ = true;
  }
  inline std::size_t Button::width() const noexcept
  {
    return this->width_;
  }

  inline void Button::height(std::size_t height) noexcept
  {
    this->height_ = height;
    this->image_is_out_of_date_ = true;
  }
  inline std::size_t Button::height() const noexcept
  {
    return this->height_;
  }
};
#endif
