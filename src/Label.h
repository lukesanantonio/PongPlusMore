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
#include "CachedSurface.h"
namespace pong
{
  /*!
   * \brief Class wrapping renderable text.
   *
   * It also features a cache which stores the rendered text and only
   * regenerates it if necessary. Very useful.
   */
  class Label : public CachedSurface
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
     */
    explicit Label(const std::string& text = "",
                   std::size_t text_height = 24,
                   math::vector pos = math::vector()) noexcept;
    /*!
     * \brief Free's the text cache if necessary.
     */
    ~Label() noexcept = default;

    /*!
     * \brief Copy constructor.
     *
     * Does not copy the cache.
     */
    Label(const Label&) noexcept;
    /*!
     * \brief Move constructor.
     *
     * Moves the cache from the object passed in, notably.
     */
    Label(Label&&) noexcept;

    /*!
     * \brief Copy assignment operator.
     *
     * Does not copy the cache, but invalidates it, just in case.
     */
    Label& operator=(const Label&) noexcept;
    /*!
     * \brief Move assignment operator.
     *
     * Moves the cache from the other object to `this` one, notably.
     */
    Label& operator=(Label&&) noexcept;

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
     * \brief Returns the width of the cached text surface.
     *
     * \returns Label::cache()->w
     *
     * \post Generates the cache if necessary.
     */
    inline std::size_t getSurfaceWidth() const;
    /*!
     * \brief Returns the height of the cached text surface.
     *
     * \returns Label::cache()->h
     *
     * \post Generates the cache if necessary.
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


    virtual SDL_Surface* generateCache_private() const override;
  };

  inline std::size_t Label::getSurfaceWidth() const
  {
    return this->cache()->w;
  }
  inline std::size_t Label::getSurfaceHeight() const
  {
    return this->cache()->h;
  }

  inline void Label::text(const std::string& text) noexcept
  {
    this->text_ = text;
    this->invalidateCache();
  }
  inline std::string Label::text() const noexcept
  {
    return this->text_;
  }

  inline void Label::text_height(std::size_t text_height) noexcept
  {
    this->text_height_ = text_height;
    this->invalidateCache();
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

};
#endif
