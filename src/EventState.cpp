#include "EventState.h"
namespace pong
{
  void EventState::pollEvents() noexcept
  {
    //Make sure to reset these flags so there isn't any trouble or
    //misunderstandings.
    this->keyReleased_ = false;
    this->mouseClicked_ = false;

    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
      switch(event.type)
      {
        case SDL_MOUSEMOTION:
        {
          this->currentMousePos_.x = event.motion.x;
          this->currentMousePos_.y = event.motion.y;
          break;
        }
        case SDL_KEYUP:
        {
          this->keyReleased_ = true;
          this->lastKeyReleased_ = event.key.keysym;
          break;
        }
        case SDL_MOUSEBUTTONUP:
        {
          this->mouseClicked_ = true;

          //No, it doesn't matter which mouse button you click...
          this->lastClickPos_.x = event.button.x;
          this->lastClickPos_.y = event.button.y;
          break;
        }
        case SDL_QUIT:
        {
          this->quit_ = true;
          break;
        }
        default:
        {
          break;
        }
      }
    }
  }
};
