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
#include <iterator>
#include <type_traits>
namespace pong
{

  template <typename T, class Deleter> struct Node;

  /*!
   * \brief A depth-first search iterator.
   */
  template <bool is_const, typename NodeT, typename NodeDeleter>
  struct Node_Iterator
  {
    using difference_type = std::size_t;
    using value_type = Node<NodeT, NodeDeleter>;
    using pointer_type = typename std::conditional<is_const,
                                         const value_type*, value_type*>::type;
    using reference_type = typename std::conditional<is_const,
                                         const value_type&, value_type&>::type;
    using category_type = std::bidirectional_iterator_tag;

    explicit Node_Iterator(pointer_type r = nullptr) noexcept
                      : root_(r), current_(find_first_child(r)) {}
    explicit Node_Iterator(pointer_type r, pointer_type current) noexcept
                      : root_(r), current_(current) {}

    Node_Iterator& operator++() noexcept;
    Node_Iterator operator++(int) noexcept;
    Node_Iterator& operator--() noexcept;
    Node_Iterator operator--(int) noexcept;

    reference_type operator*() noexcept;
    pointer_type operator->() noexcept;

    inline bool operator==(const Node_Iterator&) const noexcept;
    inline bool
    operator==(const pointer_type) const noexcept;
    inline bool operator!=(const Node_Iterator&) const noexcept;
    inline bool
    operator!=(const pointer_type) const noexcept;
  private:
    pointer_type root_;
    pointer_type current_;
  };

  template <bool is_const, typename NT, class ND>
  inline bool operator==(const typename Node_Iterator<is_const, NT, ND>::pointer_type a,
                         const Node_Iterator<is_const, NT, ND>& i) noexcept
  {
    // Just change the order of the arguments.
    return i.operator==(a);
  }

  template <bool is_const, typename NT, class ND>
  inline bool operator!=(const typename Node_Iterator<is_const, NT, ND>::pointer_type a,
                         const Node_Iterator<is_const, NT, ND>& i) noexcept
  {
    // Just change the order of the arguments.
    return i != a;
  }


  template <bool is_const, typename NT, class ND>
  Node_Iterator<is_const, NT, ND>&
  Node_Iterator<is_const, NT, ND>::operator++() noexcept
  {
    if(!this->current_)
    {
      this->current_ = find_first_child(this->root_);
      return *this;
    }
    if(this->current_->next_sibling())
    {
      this->current_ = find_first_child(this->current_->next_sibling());
      return *this;
    }
    else
    {
      // Basically find a parent with a sibling.
      while(!this->current_->next_sibling())
      {
        this->current_ = this->current_->parent();
        if(!this->current_ || this->current_ == this->root_)
        {
          // We have gone up too far.
          this->current_ = nullptr;
          return *this;
        }
      }
      // When its found, start at the beginning of that next sibling.
      this->current_ = find_first_child(this->current_->next_sibling());
      return *this;
    }
  }
  template <bool is_const, typename NT, class ND>
  Node_Iterator<is_const, NT, ND>
  Node_Iterator<is_const, NT, ND>::operator++(int) noexcept
  {
    Node_Iterator<is_const, NT, ND> t = *this;
    ++(*this);
    return t;
  }
  template <bool is_const, typename NT, class ND>
  Node_Iterator<is_const, NT, ND>&
  Node_Iterator<is_const, NT, ND>::operator--() noexcept
  {
    if(!this->current_)
    {
      this->current_ = find_last_child(this->root_);
      return *this;
    }
    if(this->current_->prev_sibling())
    {
      this->current_ = find_last_child(this->current_->prev_sibling());
      return *this;
    }
    else
    {
      while(!this->current_->prev_sibling())
      {
        this->current_ = this->current_->parent();
        if(!this->current_ || this->current_ == this->root_)
        {
          // We have gone up too far.
          this->current_ = nullptr;
          return *this;
        }
      }
      this->current_ = find_last_child(this->current_->prev_sibling());
      return *this;
    }
  }
  template <bool is_const, typename NT, class ND>
  Node_Iterator<is_const, NT, ND>
  Node_Iterator<is_const, NT, ND>::operator--(int) noexcept
  {
    Node_Iterator<is_const, NT, ND> t = *this;
    ++(*this);
    return t;
  }

  template <bool is_const, typename NT, class ND>
  auto Node_Iterator<is_const, NT, ND>::operator*() noexcept -> reference_type
  {
    return *this->current_;
  }
  template <bool is_const, typename NT, class ND>
  auto Node_Iterator<is_const, NT, ND>::operator->() noexcept -> pointer_type
  {
    return this->current_;
  }

  template <bool is_const, typename NT, class ND>
  bool Node_Iterator<is_const, NT, ND>::operator==
                      (const Node_Iterator<is_const, NT, ND>& i) const noexcept
  {
    return this->current_ == i.current_;
  }
  template <bool is_const, typename NT, class ND>
  bool Node_Iterator<is_const, NT, ND>::operator==
                                          (const pointer_type p) const noexcept
  {
    return this->current_ == p;
  }
  template <bool is_const, typename NT, class ND>
  bool Node_Iterator<is_const, NT, ND>::operator!=
                      (const Node_Iterator<is_const, NT, ND>& i) const noexcept
  {
    return !(*this == i);
  }
  template <bool is_const, typename NT, class ND>
  bool Node_Iterator<is_const, NT, ND>::operator!=
                                          (const pointer_type p) const noexcept
  {
    return !(*this == p);
  }
}
