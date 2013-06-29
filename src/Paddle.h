/*!
 * \file Paddle.h
 * \brief Declares the Paddle class and a PaddleSignals class for events
 * (probably).
 */
#ifndef ULTIMATE_PONG_PADDLE_H
#define ULTIMATE_PONG_PADDLE_H
#include <memory>
#include <SDL/SDL.h>
#include "vector.hpp"
#include "PaddleController.hpp"
#include "CachedSurface.h"
namespace pong
{
  /*!
   * \brief A concrete (base) class for paddles.
   */
  class Paddle : public CachedSurface
  {
  public:
    Paddle() = default;
    virtual ~Paddle() = default;
    Paddle(const Paddle&) = delete;
    Paddle(Paddle&&) = delete;
    Paddle& operator=(const Paddle&) = delete;
    Paddle& operator=(Paddle&&) = delete;
    /*!
     * \brief Renders the paddle onto the surface at Paddle::pos_.
     */
    void render(SDL_Surface* surface) const;
    /*!
     * \brief Uses the Paddle controller to update the paddle's location.
     *
     * \note If Paddle::controller_ is a nullptr, this function is a no-op.
     */
    void update();

    /*!
     * \brief Returns the current position of the top left corner of the paddle.
     *
     * \returns Paddle::pos_
     */
    inline math::vector position() const noexcept;
    /*!
     * \brief Sets the position of the top left corner of the paddle when it
     * needs to be rendered.
     *
     * \param pos New position after function returns.
     */
    inline void position(math::vector pos) noexcept;

    /*!
     * \brief Returns the paddle controller.
     *
     * \return Paddle::controller_
     */
    inline PaddleController* controller() const noexcept;
    /*!
     * \brief Sets the paddle controller.
     */
    inline void controller(std::shared_ptr<PaddleController>
                                                           controller) noexcept;
  private:
    /*!
     * \brief The position of the top left corner of the paddle.
     */
    math::vector pos_;

    /*!
     * The controller of the paddle.
     *
     * \sa pong::PaddleController
     */
    std::shared_ptr<PaddleController> controller_ = nullptr;

    virtual SDL_Surface* generateCache_private() const override;
  };

  inline math::vector Paddle::position() const noexcept
  {
    return this->pos_;
  }
  inline void Paddle::position(math::vector pos) noexcept
  {
    this->pos_ = pos;
  }

  inline PaddleController* Paddle::controller() const noexcept
  {
    return this->controller_.get();
  }
  inline void Paddle::controller(std::shared_ptr<PaddleController>
                                                            controller) noexcept
  {
    this->controller_ = controller;
  }
};
#endif
