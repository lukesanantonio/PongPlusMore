/*!
 * \file Button.h
 * \brief Contains declarations for the Button class.
 */
#pragma once
#include <string>
#include <boost/signals2.hpp>
#include "SDL.h"
#include "Label.h"
#include "RenderableEntity.h"
#include "EventHandler.h"
#include "render_text.h"
#include "vector.h"
namespace pong
{
  /*!
   * \brief Class wrapping clickable button.
   */
  class Button : public EventHandler, public RenderableEntity
  {
  public:
    explicit Button(const std::string& text = "",
                    math::vector pos = math::vector(),
                    std::size_t width = 0,
                    std::size_t height = 0,
                    bool enabled = true,
                    FontRenderer* font_renderer = nullptr);

    ~Button() noexcept = default;

    Button(const Button&) = delete;
    Button(Button&&) = delete;
    Button& operator=(const Button&) = delete;
    Button& operator=(Button&&) = delete;

    virtual void render(SDL_Surface*) const override;

    boost::signals2::connection executeOnClick(
                       const boost::signals2::signal<void ()>::slot_type& slot);

    virtual void handleEvent(const SDL_Event&) override;

    inline void text(const std::string& text);
    inline std::string text() const;

    inline void position(const math::vector pos) noexcept;
    inline math::vector position() const noexcept;

    inline void width(std::size_t width) noexcept;
    inline std::size_t width() const noexcept;

    inline void height(std::size_t height) noexcept;
    inline std::size_t height() const noexcept;

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
  inline void Button::position(const math::vector pos) noexcept
  {
    this->pos_ = pos;
  }
  /*!
   * \brief Returns the internal position of the button.
   *
   * \returns Button::pos_
   */
  inline math::vector Button::position() const noexcept
  {
    return this->pos_;
  }

  /*!
   * \brief Sets the width of the button to render.
   */
  inline void Button::width(std::size_t width) noexcept
  {
    this->width_ = width;
  }
  /*!
   * \brief Returns the width of the button.
   *
   * \returns Button::width_
   */
  inline std::size_t Button::width() const noexcept
  {
    return this->width_;
  }

  /*!
   * \brief Sets the height of the button to render.
   */
  inline void Button::height(std::size_t height) noexcept
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
  inline std::size_t Button::height() const noexcept
  {
    return this->height_;
  }

  /*!
   * \brief Sets whether the button can be clicked.
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
