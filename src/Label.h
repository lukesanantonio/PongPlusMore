#ifndef ULTIMATE_PONG_LABEL_H
#define ULTIMATE_PONG_LABEL_H
#include <string>
#include <SDL/SDL.h>
#include "vector.hpp"
namespace pong
{
  class Label
  {
  public:
    explicit Label(const std::string& text = "",
                   std::size_t text_size = 24,
                   math::vector pos = math::vector());
    ~Label() noexcept;
    Label(const Label&) = delete;
    Label(Label&&) = delete;
    Label& operator=(const Label&) = delete;
    Label& operator=(Label&&) = delete;
    void render(SDL_Surface* surface) const;
    void cacheSurface() const;
    SDL_Surface* getCachedSurface() const noexcept;
    void setText(const std::string& text) noexcept;
    std::string getText() const noexcept;
    void setTextSize(std::size_t text_size) noexcept;
    std::size_t getTextSize() const noexcept;
    void setPosition(math::vector pos) noexcept;
    math::vector getPosition() const noexcept;
  private:
    std::string text_;
    std::size_t text_size_;
    math::vector pos_;    
    mutable SDL_Surface* cached_surface_ = nullptr;
    mutable bool cache_out_of_date_ = true;
  };
};
#endif
