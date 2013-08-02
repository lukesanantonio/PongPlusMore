/*!
 * \file Label.h
 * \brief Contains declarations for the Label class.
 */
#pragma once
#include <string>
#include "SDL.h"
#include "CachedSurface.h"
#include "RenderableEntity.h"
#include "vector.h"
#include "render_text.h"
namespace pong
{
  /*!
   * \brief Class wrapping renderable text.
   */
  class Label : public CachedSurface, public RenderableEntity
  {
  public:
    explicit Label(const std::string& text = "",
                   std::size_t text_height = 24,
                   math::vector pos = math::vector(),
                   FontRenderer* font_renderer = nullptr) noexcept;

    Label(const std::string& text,
          std::size_t text_height,
          math::vector pos,
          SDL_Color text_color,
          SDL_Color back_color,
          FontRenderer* font_renderer) noexcept;

    /*!
     * \brief Free's the text cache if necessary.
     */
    ~Label() noexcept = default;

    Label(const Label&) noexcept;
    Label(Label&&) noexcept;

    Label& operator=(const Label&) noexcept;
    Label& operator=(Label&&) noexcept;

    virtual void render(SDL_Surface* surface) const override;
    void render(SDL_Surface* surface, math::vector pos) const;

    inline std::size_t getSurfaceWidth() const;
    inline std::size_t getSurfaceHeight() const;

    inline void text(const std::string& text) noexcept;
    inline std::string text() const noexcept;

    inline void text_height(std::size_t text_height) noexcept;
    inline std::size_t text_height() const noexcept;

    inline void position(math::vector pos) noexcept;
    inline math::vector position() const noexcept;

    inline void text_color(SDL_Color text_color) noexcept;
    inline SDL_Color text_color() const noexcept;

    inline void back_color(SDL_Color back_color) noexcept;
    inline SDL_Color back_color() const noexcept;

    inline void font_renderer(FontRenderer* font_renderer) noexcept;
    inline FontRenderer* font_renderer() const noexcept;
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
     * \brief The color of the text in the SDL_Surface generated of the text.
     */
    SDL_Color text_color_;
    /*!
     * \brief The background color, (anything not text) in the SDL_Surface
     * cached and rendered.
     */
    SDL_Color back_color_;

    /*!
     * \brief The fontrenderer implementation...
     */
    FontRenderer* font_renderer_ = nullptr;

    virtual SDL_Surface* generateCache_private() const override;
  };

  /*!
   * \brief Returns the width of the cached text surface.
   *
   * \returns Label::cache()->w
   *
   * \post Generates the cache if necessary.
   * \warning This function requires a valid FontRenderer, otherwise it crashes.
   */
  inline std::size_t Label::getSurfaceWidth() const
  {
    return this->cache()->w;
  }
  /*!
   * \brief Returns the height of the cached text surface.
   *
   * \returns Label::cache()->h
   *
   * \post Generates the cache if necessary.
   * \warning This function requires a valid FontRenderer, otherwise it crashes.
   */
  inline std::size_t Label::getSurfaceHeight() const
  {
    return this->cache()->h;
  }

  /*!
   * \brief Sets the text of the label.
   *
   * \post Invalidates the surface.
   */
  inline void Label::text(const std::string& text) noexcept
  {
    this->text_ = text;
    this->invalidateCache();
  }
  /*!
   * \brief Ges the text of the label.
   *
   * \returns Label::text_
   */
  inline std::string Label::text() const noexcept
  {
    return this->text_;
  }

  /*!
   * \brief Sets the text height of the label.
   *
   * \post Invalidates the surface.
   */
  inline void Label::text_height(std::size_t text_height) noexcept
  {
    this->text_height_ = text_height;
    this->invalidateCache();
  }
  /*!
   * \brief Returns the text height of the label.
   *
   * \returns Label::text_height_
   */
  inline std::size_t Label::text_height() const noexcept
  {
    return this->text_height_;
  }

  /*!
   * \brief Sets the position of the top left corner of the surface.
   *
   * Does not invalidate the surface.
   */
  inline void Label::position(math::vector pos) noexcept
  {
    this->pos_ = pos;
  }
  /*!
   * \brief Returns the position of the top left corner of the surface.
   *
   * \returns Label::pos_
   */
  inline math::vector Label::position() const noexcept
  {
    return this->pos_;
  }

  /*!
   * \brief Sets the color of the text.
   *
   * \post Invalidates the cache.
   */
  inline void Label::text_color(SDL_Color text_color) noexcept
  {
    this->text_color_ = text_color;
    this->invalidateCache();
  }
  /*!
   * \brief Returns the color of the text.
   *
   * \returns Label::text_color_
   */
  inline SDL_Color Label::text_color() const noexcept
  {
    return this->text_color_;
  }

  /*!
   * \brief Sets the color of the background of the image blitted.
   *
   * \post Invalidates the surface.
   */
  inline void Label::back_color(SDL_Color back_color) noexcept
  {
    this->back_color_ = back_color;
    this->invalidateCache();
  }
  /*!
   * \brief Returns the color of the background of the image blitted.
   *
   * \returns Label::back_color_
   */
  inline SDL_Color Label::back_color() const noexcept
  {
    return this->back_color_;
  }

  /*!
   * \brief Sets the font renderer implementation to use.
   *
   * \post Invalidates the cache.
   */
  inline void Label::font_renderer(FontRenderer* font_renderer) noexcept
  {
    this->font_renderer_ = font_renderer;
    this->invalidateCache();
  }
  /*!
   * \brief Returns the font renderer.
   */
  inline FontRenderer* Label::font_renderer() const noexcept
  {
    return this->font_renderer_;
  }
};
