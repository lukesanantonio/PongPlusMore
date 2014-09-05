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
#include <cstdint>
#include <vector>
#include "Object.h"
#include "ObjectManager.h"
#include <boost/variant.hpp>
namespace pong
{
  enum class severity
  {
    info,
    warning,
    error,
    unspecified
  };

  enum class ActionType
  {
    Null,
    ObjectCreation,
    ObjectDeletion
  };
  struct NullAction {};
  struct LogAction
  {
    severity severity;
    std::string msg;
  };
  struct ObjectCreationAction
  {
    Object obj;
    using callback_t = std::function<void (id_type)>;
    callback_t callback;
  };
  struct ObjectDeletionAction
  {
    id_type id;
  };
  using ServerAction = boost::variant<NullAction,
                                      LogAction,
                                      ObjectCreationAction,
                                      ObjectDeletionAction>;

  struct Server
  {
    Server() noexcept = default;
    virtual ~Server() noexcept = default;

    /*!
     * \brief Sets the destination of the object making it a paddle if it isn't
     * already.
     *
     * \throws std::out_of_range if an object with that id doesn't exist.
     */
    virtual void set_destination(id_type id, math::vector<double> dest) = 0;
    /*!
     * \brief Sets the velocity of the object making it a ball if it isn't
     * already.
     *
     * \throw std::out_of_range if an object with that id doesn't exist.
     */
    virtual void set_velocity(id_type id, math::vector<double> vel) = 0;

    /*!
     * \brief Finds some object by id, returning a copy of it.
     *
     * \throws std::out_of_range if an object with that id doesn't exist.
     */
    virtual Object find_object(id_type id) const = 0;

    /*!
     * \brief Returns the id of every object in the world currently.
     */
    virtual std::vector<id_type> objects() const noexcept = 0;

    virtual void step() noexcept = 0;

    virtual void log(const severity& s, const std::string& msg) noexcept = 0;

    inline void enqueue_action(const ServerAction& a) noexcept;
    inline void enqueue_object_creation(const Object& o,
                                  ObjectCreationAction::callback_t c) noexcept;
    inline void enqueue_object_deletion(id_type id) noexcept;

  protected:
    std::queue<ServerAction> action_queue_;
  };

  inline void Server::enqueue_action(const ServerAction& a) noexcept
  {
    this->action_queue_.push(a);
  }
  inline void Server::enqueue_object_creation(const Object& obj,
                                   ObjectCreationAction::callback_t c) noexcept
  {
    ObjectCreationAction a;
    a.obj = obj;
    a.callback = c;
    this->enqueue_action(a);
  }
  inline void Server::enqueue_object_deletion(id_type id) noexcept
  {
    ObjectDeletionAction a;
    a.id = id;
    this->enqueue_action(a);
  }
}
