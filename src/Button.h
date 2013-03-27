/*!
 * \file Button.h
 * \brief Declarations for the button class.
 */
#ifndef ULTIMATE_PONG_BUTTON_H
#define ULTIMATE_PONG_BUTTON_H
#include <string>
#include <SDL/SDL.h>
namespace pong
{
  /*!
   * \brief A button class to wrap everything about one button.
   *
   * That includes everything from a centered label to click detection based
   * on a provided signal-slot system.
   *
   * \todo Remove the x_ and y_ members in favor of a pong::vec.
   */
  class Button
  {
  public:
    /*!
     * \brief Initializes a button with some initial crap.
     *
     * \param x The x position of the top left corner of the button in SDL
     * window space.
     * \param y The y position of the top left corner of the button in SDL
     * window space.
     *
     * \param label The label to be centered on the button.
     */
    Button(int x, int y, const std::string& label);
    
    /*!
     * \brief Default constructor.
     *
     * Sets Button::x_ = 0, Button::y_ = 0, and Button::label_ = "". That is,
     * on the `this` object.
     */
    Button();
    
    /*!
     * \brief Right now the destructor needs only to be trivial.
     */
    ~Button() noexcept = default;
    
    /*!
     * \brief Copy constructor, deleted, for now.
     */
    Button(const Button&) = delete;
    
    /*!
     * \brief Move constructor, deleted, for now.
     */
    Button(Button&&) = delete;
    
    /*!
     * \brief Copy assignment operator, deleted, for now.
     */
    Button& operator=(const Button&) = delete;
    /*!
     * \brief Move assignment operator, deleted, for now.
     */
    Button& operator=(Button&&) = delete;
  private:
    /*!
     * \brief The image of the button, the width and height are taken from
     * this image...
     */
    SDL_Surface* image_;
    
    /*!
     * \brief The x position of the top left corner of the button in SDL window
     * space.
     */
    int x_;
    /*!
     * \brief The y position of the top left corner of the button in SDL window
     * space.
     */
    int y_;
    
    /*!
     * \brief The centered text on the button.
     */
    std::string label_;
  };
};
#endif
