/*!
 * \file Label.h
 * \brief Contains declarations for the Label class.
 */
#ifndef ULTIMATE_PONG_LABEL_H
#define ULTIMATE_PONG_LABEL_H
#include <string>
#include <SDL/SDL.h>
#include "vector.hpp"
#include "render_text.h"
namespace pong
{
  /*!
   * \brief Class wrapping renderable text.
   *
   * It also features a cache which stores the rendered text and only
   * regenerates it if necessary. Very useful.
   */
  class Label
  {
  public:
    /*!
     * \brief Initializes a label with everything necessary to start rendering.
     *
     * \param text The text to display. Passed to pong::render_text.
     * \param text_height The height of the text in pixels. Passed to
     * pong::render_text.
     * \param pos The position of the top left corner where the surface will be
     * blitted.
     * \param invert Whether or not the image should be inverted. (Inverting
     * the image will make the text black. This is useful for button's
     * basically.)
     */
    explicit Label(const std::string& text = "",
                   std::size_t text_height = 24,
                   math::vector pos = math::vector(),
                   bool invert = false) noexcept;
    /*!
     * \brief Free's the text cache if necessary.
     */
    ~Label() noexcept;

    /*!
     * \brief Copy constructor.
     *
     * Does not copy the cache.
     */
    Label(const Label&) noexcept;
    /*!
     * \brief Deleted move constructor.
     */
    Label(Label&&) = delete;

    /*!
     * \brief Copy assignment operator.
     *
     * Does not copy the cache.
     */
    Label& operator=(const Label&) noexcept;
    /*!
     * \brief Deleted move assignment operator.
     */
    Label& operator=(Label&&) = delete;

    /*!
     * \brief Renders an SDL_Surface containing the text of the Label onto the
     * passed in surface so that the top left corner of the text is at
     * Label::pos_.
     *
     * Basically calls Label::render(SDL_Surface*, math::vector pos) with the
     * second argument being Label::pos_.
     */
    void render(SDL_Surface* surface) const;
    /*!
     * \brief Renders the label at the position passed in, instead of the
     * internally stored one.
     *
     * \note This function will only regenerate the text surface if necessary.
     */
    void render(SDL_Surface* surface, math::vector pos) const;
    /*!
     * \brief Generates and caches the surface of rendered text.
     *
     * \note If the cache is up to date, this function is a no-op. Therefore
     * this function can be called without worry of a bottleneck whenever an
     * up to date surface is needed.
     *
     * \post Label::cache_out_of_date == `false`
     */
    void generateSurface() const;

    /*!
     * \brief Returns the width of the cached text surface.
     *
     * \returns Label::cached_surface_->w
     */
    inline std::size_t getSurfaceWidth() const;
    /*!
     * \brief Returns the height of the cached text surface.
     *
     * \returns Label::cached_surface_->h
     */
    inline std::size_t getSurfaceHeight() const;

    /*!
     * \brief Sets the text of the label.
     *
     * \post Invalidates the surface.
     */
    inline void text(const std::string& text) noexcept;
    /*!
     * \brief Ges the text of the label.
     *
     * \returns Label::text_
     */
    inline std::string text() const noexcept;

    /*!
     * \brief Sets the text height of the label.
     *
     * \post Invalidates the surface.
     */
    inline void text_height(std::size_t text_height) noexcept;
    /*!
     * \brief Returns the text height of the label.
     *
     * \returns Label::text_height_
     */
    inline std::size_t text_height() const noexcept;

    /*!
     * \brief Sets the position of the top left corner of the surface.
     *
     * Does not invalidate the surface.
     */
    inline void position(math::vector pos) noexcept;
    /*!
     * \brief Returns the position of the top left corner of the surface.
     *
     * \returns Label::pos_
     */
    inline math::vector position() const noexcept;

    /*!
     * \brief Sets whether the image should be inverted or not.
     *
     * \post Invalidates the surface.
     */
    inline void invert(bool invert) noexcept;
    /*!
     * \brief Returns whether or not the image will be inverted when rendered.
     *
     * \returns Label::invert_
     */
    inline bool invert() const noexcept;
  private:
    /*!
     * \brief The text of the label.
     *
     * \sa Label::text_height_
     */
    std::string text_;

    /*!
     * \brief The height in pixels of the label (text).
     *
     * \sa Label::text_
     */
    std::size_t text_height_;

    /*!
     * \brief The position of the top left corner of the label surface.
     *
     * \sa Label::cached_surface_
     */
    math::vector pos_;

    /*!
     * \brief The surface rendered.
     *
     * Cached so that a call to pong::render_text isn't necessary every time
     * the client calls Label::render().
     *
     * \sa Label::cache_out_of_date_
     */
    mutable SDL_Surface* cached_surface_ = nullptr;

    /*!
     * \brief True if the rendered text is out of date.
     *
     * \sa Label::cached_surface_
     */
    mutable bool cache_out_of_date_ = true;

    /*!
     * \brief Whether or not the text color should be inverted from the default
     * white. (An inversion would make the text black on a white background.)
     */
    bool invert_ = false;
  };

  inline std::size_t Label::getSurfaceWidth() const
  {
    this->generateSurface();
    return this->cached_surface_->w;
  }
  inline std::size_t Label::getSurfaceHeight() const
  {
    this->generateSurface();
    return this->cached_surface_->h;
  }

  inline void Label::text(const std::string& text) noexcept
  {
    this->text_ = text;
    this->cache_out_of_date_ = true;
  }
  inline std::string Label::text() const noexcept
  {
    return this->text_;
  }

  inline void Label::text_height(std::size_t text_height) noexcept
  {
    this->text_height_ = text_height;
    this->cache_out_of_date_ = true;
  }
  inline std::size_t Label::text_height() const noexcept
  {
    return this->text_height_;
  }

  inline void Label::position(math::vector pos) noexcept
  {
    this->pos_ = pos;
  }
  inline math::vector Label::position() const noexcept
  {
    return this->pos_;
  }

  inline void Label::invert(bool invert) noexcept
  {
    this->invert_ = invert;
    this->cache_out_of_date_ = true;
  }
  inline bool Label::invert() const noexcept
  {
    return this->invert_;
  }
};
#endif
