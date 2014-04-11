/*
 * PpM - Pong Plus More - A pong clone full of surprises written with C++11.
 * Copyright (C) 2013  Luke San Antonio
 *
 * You can contact me (Luke San Antonio) at lukesanantonio@gmail.com!
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once
#include "Server.h"
#include "Quadtree.h"
namespace pong
{
  struct LocalServer : public Server
  {
    LocalServer(Volume v) noexcept : quadtree_(v, 3, 5) {}
    ~LocalServer() noexcept = default;

    inline id_type createPaddle(const Volume& v) noexcept;
    inline id_type createBall(const Volume& v) noexcept;

    void setDestination(id_type, math::vector<double>) override;
    void setVelocity(id_type, math::vector<double>) override;

    Object getObject(id_type) const override;
    std::vector<id_type> objects() const noexcept override;

    const Quadtree& quadtree() const noexcept { return this->quadtree_; }

    void step() noexcept override;
  private:
    Quadtree quadtree_;
  };

  inline id_type LocalServer::createPaddle(const Volume& v) noexcept
  {
    return this->quadtree_.makePaddle(v);
  }
  inline id_type LocalServer::createBall(const Volume& v) noexcept
  {
    return this->quadtree_.makeBall(v);
  }
}
