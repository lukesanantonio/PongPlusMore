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
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = Node<NodeT, NodeDeleter>;
    using difference_type = std::size_t;
    using pointer = std::conditional_t<is_const,
                                       const value_type*, value_type*>;
    using reference = std::conditional_t<is_const,
                                         const value_type&, value_type&>;

    explicit Node_Iterator(pointer r = nullptr) noexcept
                      : root_(r), current_(find_first_child(r)) {}

    explicit Node_Iterator(pointer r, pointer current) noexcept
                      : root_(r), current_(current) {}

    template <bool param_const>
    Node_Iterator(
               const Node_Iterator<param_const, NodeT, NodeDeleter>&) noexcept;

    Node_Iterator& operator++() noexcept;
    Node_Iterator operator++(int) noexcept;
    Node_Iterator& operator--() noexcept;
    Node_Iterator operator--(int) noexcept;

    reference operator*() const noexcept;
    pointer operator->() const noexcept;
  private:
    pointer root_;
    pointer current_;
  };

  /*!
   * This function will only not go from const to nonconst but it will work
   * the other way.
   */
  template <bool is_const, typename NT, class ND>
  template <bool param_const>
  Node_Iterator<is_const, NT, ND>::Node_Iterator(
                          const Node_Iterator<param_const, NT, ND>& n) noexcept
                          : root_(n.root_), current_(n.current_) {}

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

  template <bool is_const, typename NT, class ND> auto
  Node_Iterator<is_const, NT, ND>::operator*() const noexcept -> reference
  {
    return *this->current_;
  }
  template <bool is_const, typename NT, class ND> auto
  Node_Iterator<is_const, NT, ND>::operator->() const noexcept -> pointer
  {
    return this->current_;
  }

  template <bool is_const1, bool is_const2, typename NT, class ND>
  bool operator==(const Node_Iterator<is_const1, NT, ND>& i1,
                  const Node_Iterator<is_const2, NT, ND>& i2) noexcept
  {
    return &(*i1) == &(*i2);
  }
  template <bool is_const1, bool is_const2, typename NT, class ND>
  bool operator!=(const Node_Iterator<is_const1, NT, ND>& i1,
                  const Node_Iterator<is_const2, NT, ND>& i2) noexcept
  {
    return !(i1 == i2);
  }
}
