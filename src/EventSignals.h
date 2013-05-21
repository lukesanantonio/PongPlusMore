#ifndef ULTIMATE_PONG_EVENT_SIGNALS_H
#define ULTIMATE_PONG_EVENT_SIGNALS_H
#include <SDL/SDL.h>
#include <boost/signals2.hpp>
#include "vector.hpp"
namespace pong
{
  struct EventSignals
  {
    boost::signals2::signal<void (math::vector)> on_mouse_click;
    boost::signals2::signal<void (math::vector)> on_mouse_motion;
    boost::signals2::signal<void ()> on_quit;
  };
  /*!
   * \brief Checks for events with SDL and fires the corresponding signal in
   * the EventSignals struct passed in if necessary.
   */
  void handleEvents(EventSignals& signals);
};
#endif
