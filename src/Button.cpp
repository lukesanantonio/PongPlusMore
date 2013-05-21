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
                 std::size_t text_height) :
                 label_(text, text_height),
                 pos_(pos),
                 width_(width),
                 height_(height)
  {
    this->label_.invert(true);
  }
  Button::~Button()
  {
    if(this->image_)
    {
      SDL_FreeSurface(this->image_);
    }
  }

  void Button::render(SDL_Surface* surface) const
  {
    //Generate our image if necessary.
    this->generateImage();

    //Render image!
    SDL_Rect button_dest;
    button_dest.x = this->pos_.x;
    button_dest.y = this->pos_.y;
    SDL_BlitSurface(this->image_, NULL, surface, &button_dest);

    //Find where to render the label!
    math::vector label_pos;
    label_pos.x = center(this->pos_.x, this->width_,
                         this->label_.getSurfaceWidth());
    label_pos.y = center(this->pos_.y, this->height_,
                         this->label_.getSurfaceHeight());

    //Render the label.
    this->label_.render(surface, label_pos);
  }

  void Button::generateImage() const
  {
    if(this->image_is_out_of_date_)
    {
      //Make sure we don't leak any memory.
      if(this->image_)
      {
        SDL_FreeSurface(this->image_);
      }

      //Generate the rectangle.
      SDL_Color white;
      white.r = 0xff;
      white.g = 0xff;
      white.b = 0xff;
      this->image_ = generateRectangle(this->width_, this->height_, white);

      //Don't do this again.
      this->image_is_out_of_date_ = false;
    }
  }
  boost::signals2::connection Button::executeOnClick(
                        const boost::signals2::signal<void ()>::slot_type& slot)
  {
    return this->on_click_.connect(slot);
  }
  void Button::addHandler(EventSignals& signals) const
  {
    //Add the signal to be called when handling events.
    signals.on_mouse_click.connect([this](math::vector point)
    {
      //Check to see if the button occupies the point... Sooo:
      if(point.x <= this->pos_.x + width_ && point.x >= this->pos_.x &&
         point.y <= this->pos_.y + height_ && point.y >= this->pos_.y)
      {
        //Do everything on our to-do list.
        this->on_click_();
      }
    });
  }
};
