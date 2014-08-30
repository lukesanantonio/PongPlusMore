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
#include <boost/signals2.hpp>
namespace pong
{
  enum class ActionType
  {
    ObjectCreation,
    ObjectDeletion
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
  using ServerAction = boost::variant<ObjectCreationAction,
                                      ObjectDeletionAction>;

  struct ModifiedObjectReference;
  struct LocalServer : public Server
  {
    LocalServer(Volume v) noexcept;
    ~LocalServer() noexcept;

    id_type insert(const Object& o) noexcept;

    inline void enqueue_action(const ServerAction& a) noexcept;
    inline void enqueue_object_creation(const Object& o,
                                  ObjectCreationAction::callback_t c) noexcept;
    inline void enqueue_object_deletion(id_type id) noexcept;

    void set_destination(id_type, math::vector<double>) override;
    void set_velocity(id_type, math::vector<double>) override;

    Object find_object(id_type) const override;
    std::vector<id_type> objects() const noexcept override;

    const Quadtree& quadtree() const noexcept { return this->quadtree_; }

    void step() noexcept override;

    using wall_observer_signal_t =
                          boost::signals2::signal<void (VolumeSides s,
                                                        id_type id,
                                                        Quadtree& q)>;
    using connection_t = boost::signals2::connection;

    using wall_observer_t = wall_observer_signal_t::slot_type;

    inline
    connection_t add_wall_collision_observer(const wall_observer_t&) noexcept;
  private:
    wall_observer_signal_t obs_;

    Quadtree quadtree_;

    std::queue<ServerAction> action_queue_;

    void react(ModifiedObjectReference& obj) noexcept;
    void raytrace(id_type id) noexcept;
  };

  inline void LocalServer::enqueue_action(const ServerAction& a) noexcept
  {
    this->action_queue_.push(a);
  }
  inline void LocalServer::enqueue_object_creation(const Object& obj,
                                   ObjectCreationAction::callback_t c) noexcept
  {
    ObjectCreationAction a;
    a.obj = obj;
    a.callback = c;
    this->enqueue_action(a);
  }
  inline void LocalServer::enqueue_object_deletion(id_type id) noexcept
  {
    ObjectDeletionAction a;
    a.id = id;
    this->enqueue_action(a);
  }

  inline auto LocalServer::add_wall_collision_observer(
                          const wall_observer_t& slot) noexcept -> connection_t
  {
    return this->obs_.connect(slot);
  }
}
