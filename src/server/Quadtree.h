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
#include <memory>
#include <array>
#include <vector>
#include "common/util.h"
namespace pong
{
  template <typename T,
            class Deleter = typename std::unique_ptr<T>::deleter_type>
  struct Node
  {
  public:
    /*!
     * \brief A depth-first search iterator.
     */
    struct Iterator
    {
      // TODO: Add actual iterator type aliases.

      explicit Iterator(Node* r = nullptr) noexcept
                        : root_(r), current_(find_first_child(r)) {}
      explicit Iterator(Node* r, Node* current) noexcept
                        : root_(r), current_(current) {}

      Iterator& operator++() noexcept;
      Iterator operator++(int) noexcept;
      Iterator& operator--() noexcept;
      Iterator operator--(int) noexcept;

      Node& operator*() noexcept;
      Node* operator->() noexcept;

      inline bool operator==(const Iterator&) const noexcept;
      inline bool operator==(const Node*) const noexcept;
      inline bool operator!=(const Iterator&) const noexcept;
      inline bool operator!=(const Node*) const noexcept;
    private:
      Node* root_;
      Node* current_;
    };

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

    inline Node* parent() const noexcept { return this->parent_; }
    inline void parent(Node* p) noexcept { this->parent_ = p; }

    inline Node* next_sibling() const noexcept { return this->next_sibling_; }
    inline void next_sibling(Node* n) noexcept { this->next_sibling_ = n; }

    inline Node* prev_sibling() const noexcept { return this->prev_sibling_; }
    inline void prev_sibling(Node* n) noexcept { this->prev_sibling_ = n; }

    inline Iterator begin() noexcept { return Iterator(this); }
    inline Iterator end() noexcept { return Iterator(this, nullptr); }
  private:
    std::unique_ptr<T, Deleter> data_;
    std::vector<std::unique_ptr<Node> > children_;

    Node* parent_;
    Node* next_sibling_;
    Node* prev_sibling_;
  };

  template <typename T, class Deleter> inline bool
  operator==(const Node<T, Deleter>* a,
             const typename Node<T, Deleter>::Iterator& i) noexcept
  {
    return i == a;
  }

  template <typename T, class Deleter> inline bool
  operator!=(const Node<T, Deleter>* a,
             const typename Node<T, Deleter>::Iterator& i) noexcept
  {
    return i != a;
  }

  template <typename T, class Deleter>
  Node<T, Deleter>* find_first_child(Node<T, Deleter>* n) noexcept
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
  Node<T, Deleter>* find_last_child(Node<T, Deleter>* n) noexcept
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

  template <typename T, class Deleter> typename Node<T, Deleter>::Iterator&
  Node<T, Deleter>::Iterator::operator++() noexcept
  {
    if(!this->current_)
    {
      this->current_ = find_first_child(this->root_);
      return *this;
    }
    if(this->current_->next_sibling_)
    {
      this->current_ = find_first_child(this->current_->next_sibling_);
      return *this;
    }
    else
    {
      // Basically find a parent with a sibling.
      while(!this->current_->next_sibling_)
      {
        this->current_ = this->current_->parent_;
        if(!this->current_ || this->current_ == this->root_)
        {
          // We have gone up too far.
          this->current_ = nullptr;
          return *this;
        }
      }
      // When its found, start at the beginning of that next sibling.
      this->current_ = find_first_child(this->current_->next_sibling_);
      return *this;
    }
  }
  template <typename T, class Deleter> typename Node<T, Deleter>::Iterator
  Node<T, Deleter>::Iterator::operator++(int) noexcept
  {
    Node::Iterator t = *this;
    ++(*this);
    return t;
  }
  template <typename T, class Deleter>
  typename Node<T, Deleter>::Iterator&
  Node<T, Deleter>::Iterator::operator--() noexcept
  {
    if(!this->current_)
    {
      this->current_ = find_last_child(this->root_);
      return *this;
    }
    if(this->current_->prev_sibling_)
    {
      this->current_ = find_last_child(this->current_->prev_sibling_);
      return *this;
    }
    else
    {
      while(!this->current_->prev_sibling_)
      {
        this->current_ = this->current_->parent_;
        if(!this->current_ || this->current_ == this->root_)
        {
          // We have gone up too far.
          this->current_ = nullptr;
          return *this;
        }
      }
      this->current_ = find_last_child(this->current_->prev_sibling_);
      return *this;
    }
  }
  template <typename T, class Deleter> typename Node<T, Deleter>::Iterator
  Node<T, Deleter>::Iterator::operator--(int) noexcept
  {
    Node::Iterator t = *this;
    ++(*this);
    return t;
  }

  template <typename T, class Deleter>
  Node<T, Deleter>& Node<T, Deleter>::Iterator::operator*() noexcept
  {
    return *this->current_;
  }
  template <typename T, class Deleter>
  Node<T, Deleter>* Node<T, Deleter>::Iterator::operator->() noexcept
  {
    return this->current_;
  }

  template <typename T, class Deleter> inline bool
  Node<T, Deleter>::Iterator::operator==
                                       (const Node::Iterator& i) const noexcept
  {
    return this->current_ == i.current_;
  }
  template <typename T, class Deleter> inline bool
  Node<T, Deleter>::Iterator::operator==(const Node* p) const noexcept
  {
    return this->current_ == p;
  }
  template <typename T, class Deleter> inline bool
  Node<T, Deleter>::Iterator::operator!=
                                       (const Node::Iterator& i) const noexcept
  {
    return !(*this == i);
  }
  template <typename T, class Deleter> inline bool
  Node<T, Deleter>::Iterator::operator!=(const Node* p) const noexcept
  {
    return !(*this == p);
  }
}
