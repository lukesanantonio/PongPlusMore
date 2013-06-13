/*!
 * \file Button.cpp
 * \brief Contains definitions for the Button class.
 */
#include "Button.h"
#include "render_text.h"
namespace pong
{
  Button::Button(const std::string& text,
                 math::vector pos,
                 std::size_t width,
                 std::size_t height,
                 bool enabled) :
                 label_(text, 24),
                 pos_(pos),
                 width_(width),
                 height_(height),
                 enabled_(enabled)
  {
  }

  void Button::render(SDL_Surface* surface) const
  {
    //Render image!
    SDL_Rect button_dest;
    button_dest.x = this->pos_.x;
    button_dest.y = this->pos_.y;
    SDL_BlitSurface(this->cache(), NULL, surface, &button_dest);

    //Find where to render the label!
    math::vector label_pos;
    label_pos.x = center(this->pos_.x, this->width_,
                         this->label_.getSurfaceWidth());
    label_pos.y = center(this->pos_.y, this->height_,
                         this->label_.getSurfaceHeight());

    //Render the label.
    this->label_.render(surface, label_pos);
  }

  SDL_Surface* Button::generateCache_private() const
  {
    //Generate the rectangle.
    SDL_Color color;
    if(this->enabled_)
    {
      color.r = 0xff;
      color.g = 0xff;
      color.b = 0xff;
    }
    else
    {
      color.r = 0x55;
      color.g = 0x55;
      color.b = 0x55;
    }
    SDL_Surface* image = generateRectangle(this->width_, this->height_, color);
    return image;
  }
  boost::signals2::connection Button::executeOnClick(
                        const boost::signals2::signal<void ()>::slot_type& slot)
  {
    return this->on_click_.connect(slot);
  }
  void Button::useSignals(EventSignals& signals) const
  {
    //Add the signal to be called when handling events.
    signals.on_mouse_click.connect([this](math::vector point)
    {
      //The button can only be clicked if it is enabled.
      if(this->enabled_)
      {
        //Check to see if the button occupies the point... Sooo:
        if(point.x <= this->pos_.x + width_ && point.x >= this->pos_.x &&
           point.y <= this->pos_.y + height_ && point.y >= this->pos_.y)
        {
          //Do everything on our to-do list.
          this->on_click_();
        }
      }
    });
  }
};
