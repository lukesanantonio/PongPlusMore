#ifndef ULTIMATE_PONG_LABEL_H
#define ULTIMATE_PONG_LABEL_H
#include <string>
#include <SDL/SDL.h>
#include "vector.hpp"
namespace pong
{
  /*!
   * \brief Class used to store a label of text and it's position.
   *
   * Basically a helper, which is far superior to storing a surface and a
   * position and then documented how they are related. This class informs the
   * compiler about the connection.
   */
  class Label
  {
  public:
    
    /*!
     * \brief Initializes everything up.
     *
     * \param text Text of the label.
     * \param text_size Size of the text in pixels.
     * \param pos Position of the top left corner in SDL window space of the
     * text.
     */
    explicit Label(const std::string& text = "",
                   std::size_t text_size = 24,
                   math::vector pos = math::vector());
    
    /*!
     * \brief Destructs the text surface cache if necessary.
     */
    ~Label() noexcept;
    
    /*!
     * \brief Deleted copy constructor.
     */
    Label(const Label&) = delete;
    
    /*!
     * \brief Deleted move constructor..
     */
    Label(Label&&) = delete;
    
    /*!
     * \brief Deleted copy assignment operator.
     */
    Label& operator=(const Label&) = delete;
    
    /*!
     * \brief Deleted move assignment operator.
     */
    Label& operator=(Label&&) = delete;
    
    /*!
     * \brief Renders the text onto the surface passed in.
     *
     * The top left corner of the text will be located at the Label::pos_ of
     * the `this` object.
     */
    void render(SDL_Surface* surface) const;
    
    /*!
     * \brief Generates the text surface to be rendered.
     *
     * This method basically allows for optimization in letting the client
     * decide when the best time to generate the surface is. And since the
     * render function will not generate a new surface unless necessary this
     * really does help.
     *
     * \note This function will run the pong::render_text function regardless
     * of Label::cache_out_of_date_.
     */
    void cacheSurface() const;
    
    /*!
     * \brief Returns the cached_surface_ of the `this` object.
     *
     * \returns Label::cached_surface_ of the `this` object. No more, no less.
     *
     * \note If you SDL_FreeSurface() the cache, you can screw yourself! =D
     */
    SDL_Surface* getCachedSurface() const noexcept;
    
    /*!
     * \brief Sets the text to render.
     *
     * \param text New text to render after this call.
     */
    void setText(const std::string& text) noexcept;
    
    /*!
     * \brief Returns the text which is being rendered as a string.
     *
     * \return Label::text_ of the `this` object.
     */
    std::string getText() const noexcept;
    
    /*!
     * \brief Sets the label's text size.
     *
     * \param text_size The new text size to render the string at.
     */
    void setTextSize(std::size_t text_size) noexcept;
    
    /*!
     * \brief Returns the text size of the string being rendered.
     *
     * \returns Label::text_size_ of the `this` object.
     */
    std::size_t getTextSize() const noexcept;
    
    /*!
     * \brief Sets the label's top left position.
     *
     * \param pos The new top left position of the string surface (image).
     */
    void setPosition(math::vector pos) noexcept;
    
    /*!
     * \brief Returns the top left position of the string surface (image).
     *
     * \returns Label::pos_ of the `this` object.
     */
    math::vector getPosition() const noexcept;
  private:
    /*!
     * \brief The text to be rendered.
     *
     * If this is an empty string, no call to pong::render_text is made.
     */
    std::string text_;
    
    /*!
     * \brief Text size
     *
     * If this is zero, no call to pong::render_text is made.
     */
    std::size_t text_size_;
    
    /*!
     * \brief The top left corner of the text to be rendered in SDL window
     * space.
     */
    math::vector pos_;    
    
    /*!
     * \brief A cached render of the text
     *
     * Used so we don't actually have to render the text every time
     * Label::render is called. This, if used, properly should speed things up
     * *a lot*.
     */
    mutable SDL_Surface* cached_surface_ = nullptr;
    
    /*!
     * \brief Whether or not the cache is up to date.
     *
     * Set on occasion when we have to re-render.
     */
    mutable bool cache_out_of_date_ = true;
  };
};
#endif
