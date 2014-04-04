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
#include <memory>
#include <vector>
#include "common/util.h"
#include "Node_Iterator.hpp"
#pragma once
namespace pong
{
  template <typename T,
            class Deleter = typename std::unique_ptr<T>::deleter_type>
  struct Node
  {
  private:
    explicit Node(Node* parent,
             Node* next_sibling = nullptr,
             Node* prev_sibling = nullptr,
             std::unique_ptr<T, Deleter> data =
                                        std::unique_ptr<T, Deleter>()) noexcept
             : data_(std::move(data)), children_(), parent_(parent),
               next_sibling_(next_sibling), prev_sibling_(prev_sibling) {}

  public:
    explicit Node(std::unique_ptr<T, Deleter> data =
                                        std::unique_ptr<T, Deleter>()) noexcept
                  : Node(nullptr, nullptr, nullptr, std::move(data)) {}

    inline std::unique_ptr<T, Deleter>& get_data() noexcept
    { return this->data_; }
    inline const std::unique_ptr<T, Deleter>& get_data() const noexcept
    { return this->data_; }

    Node* push_child() noexcept;
    Node* push_child(std::unique_ptr<T, Deleter> data) noexcept;

    inline std::vector<Node*> children() noexcept
    {
      return vector_cast<Node*>(this->children_,
      //[] (auto& e) { return e.get(); });
      std::bind(std::mem_fn(&std::unique_ptr<Node>::get),
                std::placeholders::_1));
    }
    inline std::vector<const Node*> children() const noexcept
    {
      return vector_cast<const Node*>(this->children_,
      [] (const auto& e) -> const Node* { return e.get(); });
    }

    inline bool remove_child(Node* n) noexcept
    {
      using std::begin; using std::end;
      auto iter = std::find_if(begin(this->children_), end(this->children_),
      [&](const std::unique_ptr<Node>& up)
      {
        return up.get() == n;
      });

      if(iter == end(this->children_))
      {
        return false;
      }

      this->children_.erase(iter);
      return true;
    }

    inline Node* parent() const noexcept { return this->parent_; }
    inline void parent(Node* p) noexcept { this->parent_ = p; }

    inline Node* next_sibling() const noexcept { return this->next_sibling_; }
    inline void next_sibling(Node* n) noexcept { this->next_sibling_ = n; }

    inline Node* prev_sibling() const noexcept { return this->prev_sibling_; }
    inline void prev_sibling(Node* n) noexcept { this->prev_sibling_ = n; }

    inline Node_Iterator<false, T, Deleter> begin() noexcept
    { return Node_Iterator<false, T, Deleter>(this); }
    inline Node_Iterator<false, T, Deleter> end() noexcept
    { return Node_Iterator<false, T, Deleter>(this, nullptr); }

    inline Node_Iterator<true, T, Deleter> cbegin() const noexcept
    { return Node_Iterator<true, T, Deleter>(this); }
    inline Node_Iterator<true, T, Deleter> cend() const noexcept
    { return Node_Iterator<true, T, Deleter>(this, nullptr); }
  private:
    std::unique_ptr<T, Deleter> data_;
    std::vector<std::unique_ptr<Node> > children_;

    Node* parent_;
    Node* next_sibling_;
    Node* prev_sibling_;
  };

  template <typename T, class Deleter>
  const Node<T, Deleter>* find_first_child(const Node<T, Deleter>* n) noexcept
  {
    if(!n) return nullptr;
    if(n->children().empty())
    {
      return n;
    }
    else
    {
      return find_first_child(n->children().front());
    }
  }
  template <typename T, class Deleter>
  Node<T, Deleter>* find_first_child(Node<T, Deleter>* n) noexcept
  {
    return const_cast<Node<T, Deleter>* >
                   (find_first_child(const_cast<const Node<T, Deleter>* >(n)));
  }

  template <typename T, class Deleter>
  const Node<T, Deleter>* find_last_child(const Node<T, Deleter>* n) noexcept
  {
    if(!n) return nullptr;
    if(n->children().empty())
    {
      return n;
    }
    else
    {
      return find_last_child(n->children().back());
    }
  }
  template <typename T, class Deleter>
  Node<T, Deleter>* find_last_child(Node<T, Deleter>* n) noexcept
  {
    return const_cast<Node<T, Deleter>* >
                   (find_last_child(const_cast<const Node<T, Deleter>* >(n)));
  }

  template <typename T, class Deleter>
  Node<T, Deleter>* Node<T, Deleter>::push_child() noexcept
  {
    return this->push_child(nullptr);
  }
  template <typename T, class Deleter> Node<T, Deleter>*
  Node<T, Deleter>::push_child(std::unique_ptr<T, Deleter> data) noexcept
  {
    Node* prev_sibling =
             !this->children_.empty() ? this->children_.back().get() : nullptr;

    this->children_.push_back(std::unique_ptr<Node>(
                      new Node(this, nullptr, prev_sibling, std::move(data))));

    if(prev_sibling)
    {
      prev_sibling->next_sibling(this->children_.back().get());
    }
    return this->children_.back().get();
  }
}
