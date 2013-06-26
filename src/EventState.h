/*!
 * \file EventState.h
 * \brief
 */
#ifndef ULTIMATE_PONG_EVENT_STATE_H
#define ULTIMATE_PONG_EVENT_STATE_H
#include <SDL/SDL.h>
#include "vector.hpp"
namespace pong
{
  /*!
   * \brief Manages all event state at any given update.
   *
   * It is assumed, by this class, that events will be polled every game loop.
   * Otherwise, events may and will be lost.
   *
   * Variable names are fairly crappy, but all the comments should clear
   * everything up.
   *
   * \note Make sure only one instance is constructed, this will most likely be
   * in the Game class.
   */
  class EventState
  {
  public:
    EventState() noexcept = default;

    EventState(const EventState&) noexcept = delete;
    EventState(EventState&&) noexcept = delete;

    EventState& operator=(const EventState&) noexcept = delete;
    EventState& operator=(EventState&&) noexcept = delete;

    ~EventState() noexcept = default;

    /*!
     * \brief Polls for events and updates internal state.
     */
    void pollEvents() noexcept;

    /*!
     * \brief Returns the current mouse position.
     *
     * \returns EventState::currentMousePos_;
     */
    inline math::vector mousePosition() const noexcept;

    /*!
     * \brief Returns whether a key has been pressed on this tick (game loop
     * iteration).
     *
     * \returns EventState::keyReleased_
     */
    inline bool keyHasBeenPressed() const noexcept;
    /*!
     * \brief Returns the key last pressed and released, if it hasen't been
     * released yet, it hasn't registered yet.
     *
     * \returns EventState::lastKeyReleased_
     */
    inline SDL_keysym lastKeyPressed() const noexcept;

    /*!
     * \brief Returns whether or not the user has clicked the mouse in the
     * current game loop iteration (sorry, bad wording... see class description)
     *
     * \returns EventState::mouseClicked_
     */
    inline bool mouseHasBeenClicked() const noexcept;
    /*!
     * \brief Returns the last place the user clicked, not necessary up to date.
     *
     * \returns EventState::lastClickPos_
     */
    inline math::vector lastClickLocation() const noexcept;

    /*!
     * \brief Returns whether or not the user has pressed the little 'x' button
     * sometime in the past.
     *
     * \returns EventState::quit_
     */
    inline bool WindowHasBeenClosed() const noexcept;
  private:
    /*!
     * \brief Where the mouse is currently at...
     *
     * This is changed whenever the mouse is moved. Clicking isn't necessary.
     */
    math::vector currentMousePos_;

    /*!
     * \brief Whether or not EventState::lastKeyReleased_ is valid.
     *
     * Basically whether or not the user released a key during the current
     * game loop iteration.
     */
    bool keyReleased_ = false;
    /*!
     * \brief The last released key.
     *
     * Only up to date if EventState::keyReleased_ is true.
     */
    SDL_keysym lastKeyReleased_;

    /*!
     * \brief Whether or not the user has clicked down with their mouse.
     *
     * This value determines the up-to-dateness of
     * EventState::lastClickPos_
     */
    bool mouseClicked_ = false;
    /*!
     * \brief The last clicked the location of the user with the mouse...
     *
     * Only really up to date and valid if EventState::mouseClicked_ is true.
     */
    math::vector lastClickPos_;

    /*!
     * \note This value is retained once set, even after multiple calls to
     * EventState::pollEvents().
     */
    bool quit_ = false;
  };
  inline math::vector EventState::mousePosition() const noexcept
  {
    return this->currentMousePos_;
  }
  inline bool EventState::keyHasBeenPressed() const noexcept
  {
    return this->keyReleased_;
  }
  inline SDL_keysym EventState::lastKeyPressed() const noexcept
  {
    return this->lastKeyReleased_;
  }
  inline bool EventState::mouseHasBeenClicked() const noexcept
  {
    return this->mouseClicked_;
  }
  inline math::vector EventState::lastClickLocation() const noexcept
  {
    return this->lastClickPos_;
  }
  inline bool EventState::WindowHasBeenClosed() const noexcept
  {
    return this->quit_;
  }
};
#endif