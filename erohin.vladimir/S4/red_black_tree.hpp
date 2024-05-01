#ifndef RED_BLACK_TREE_HPP
#define RED_BLACK_TREE_HPP

#include <functional>
#include <initializer_list>
#include "tree_node.hpp"
#include "tree_const_iterator.hpp"
#include "tree_iterator.hpp"

#include <iostream>

namespace erohin
{
  template< class Key, class T, class Compare = std::less< Key > >
  class RedBlackTree
  {
  public:
    using iterator = TreeIterator< Key, T >;
    using const_iterator = TreeConstIterator< Key, T >;
    using value_type = std::pair< Key, T >;
    RedBlackTree();
    RedBlackTree(const RedBlackTree< Key, T, Compare > & rhs);
    RedBlackTree(RedBlackTree< Key, T, Compare > && rhs) noexcept;
    RedBlackTree(std::initializer_list< value_type > init_list);
    template< class InputIt>
    RedBlackTree(InputIt first, InputIt last);
    ~RedBlackTree();
    RedBlackTree< Key, T, Compare > & operator=(const RedBlackTree< Key, T, Compare > & rhs);
    RedBlackTree< Key, T, Compare > & operator=(RedBlackTree< Key, T, Compare > && rhs) noexcept;
    iterator begin();
    iterator end();
    const_iterator cbegin();
    const_iterator cend();
    void clear();
    bool empty() const noexcept;
    std::pair< iterator, bool > insert(const value_type & value);
    std::pair< iterator, bool > erase(const Key & key);
    void swap(RedBlackTree & rhs);
  private:
    detail::Node< Key, T > * root_;
    Compare cmp_;
    void clear_subtree(detail::Node< Key, T > * subtree);
    void insert_balance_case1(detail::Node< Key, T > * subtree);
    void insert_balance_case2(detail::Node< Key, T > * subtree);
    void insert_balance_case3(detail::Node< Key, T > * subtree);
    void insert_balance_case4(detail::Node< Key, T > * subtree);
    void insert_balance_case5(detail::Node< Key, T > * subtree);
    void erase_balance_case1(detail::Node< Key, T > * subtree);
    detail::Node< Key, T > * find_to_change_erased(detail::Node< Key, T > * subtree);
    detail::Node< Key, T > * find_grandparent(detail::Node< Key, T > * subtree);
    detail::Node< Key, T > * find_uncle(detail::Node< Key, T > * subtree);
    detail::Node< Key, T > * find_brother(detail::Node< Key, T > * subtree);
    void rotate_left(detail::Node< Key, T > * subtree);
    void rotate_right(detail::Node< Key, T > * subtree);
    bool is_leaf(detail::Node< Key, T > * node);
  };

  template< class Key, class T, class Compare >
  RedBlackTree< Key, T, Compare >::RedBlackTree():
    root_(nullptr)
  {}

  template< class Key, class T, class Compare >
  RedBlackTree< Key, T, Compare >::RedBlackTree(const RedBlackTree< Key, T, Compare > & rhs):
    RedBlackTree(rhs.cbegin(), rhs.cend())
  {}

  template< class Key, class T, class Compare >
  RedBlackTree< Key, T, Compare >::RedBlackTree(RedBlackTree< Key, T, Compare > && rhs) noexcept:
    root_(rhs.root_)
  {
    rhs.root_ = nullptr;
  }

  template< class Key, class T, class Compare >
  RedBlackTree< Key, T, Compare >::RedBlackTree(std::initializer_list< value_type > init_list):
    RedBlackTree(init_list.begin(), init_list.end())
  {}

  template< class Key, class T, class Compare >
  template< class InputIt >
  RedBlackTree< Key, T, Compare >::RedBlackTree(InputIt first, InputIt last):
    root_(nullptr)
  {
    while (first != last)
    {
      try
      {
        insert(*(first++));
      }
      catch (...)
      {
        clear();
      }
    }
  }

  template< class Key, class T, class Compare >
  RedBlackTree< Key, T, Compare >::~RedBlackTree()
  {
    clear();
  }

  template< class Key, class T, class Compare >
  RedBlackTree< Key, T, Compare > & RedBlackTree< Key, T, Compare >::operator=(const RedBlackTree< Key, T, Compare > & rhs)
  {
    if (std::addressof(rhs) != this)
    {
      RedBlackTree< Key, T, Compare > temp(rhs);
      swap(temp);
      return *this;
    }
  }

  template< class Key, class T, class Compare >
  RedBlackTree< Key, T, Compare > & RedBlackTree< Key, T, Compare >::operator=(RedBlackTree< Key, T, Compare > && rhs) noexcept
  {
    if (std::addressof(rhs) != this)
    {
      RedBlackTree< Key, T, Compare > temp(std::move(rhs));
      swap(temp);
      return *this;
    }
  }

  template< class Key, class T, class Compare >
  TreeIterator< Key, T > RedBlackTree< Key, T, Compare >::begin()
  {
    detail::Node< Key, T > * result = root_;
    while (result->left_)
    {
      result = result->left_;
    }
    return iterator(result);
  }

  template< class Key, class T, class Compare >
  TreeIterator< Key, T > RedBlackTree< Key, T, Compare >::end()
  {
    return iterator(nullptr);
  }

  template< class Key, class T, class Compare >
  TreeConstIterator< Key, T > RedBlackTree< Key, T, Compare >::cbegin()
  {
    return const_iterator(begin().node_);
  }

  template< class Key, class T, class Compare >
  TreeConstIterator< Key, T > RedBlackTree< Key, T, Compare >::cend()
  {
    return const_iterator(end().node_);
  }

  template< class Key, class T, class Compare >
  void RedBlackTree< Key, T, Compare >::clear()
  {
    clear_subtree(root_);
    root_ = nullptr;
  }

  template< class Key, class T, class Compare >
  std::pair< TreeIterator< Key, T >, bool > RedBlackTree< Key, T, Compare >::insert(const value_type & value)
  {
    detail::Node< Key, T > * node = root_;
    if (empty())
    {
      root_ = new detail::Node< Key, T >(value, nullptr, nullptr, nullptr);
      node = root_;
    }
    else
    {
      detail::Node< Key, T > * prev = node;
      while (node)
      {
        prev = node;
        if (node->data_.first == value.first)
        {
          return std::make_pair(iterator(node), false);
        }
        else if (cmp_(value.first, node->data_.first))
        {
          node = node->left_;
        }
        else
        {
          node = node->right_;
        }
      }
      node = new detail::Node< Key, T >(value, prev, nullptr, nullptr);
      if (cmp_(node->data_.first, prev->data_.first))
      {
        prev->left_ = node;
      }
      else
      {
        prev->right_ = node;
      }
    }
    insert_balance_case1(node);
    return std::make_pair(iterator(node), true);
  }

  template< class Key, class T, class Compare >
  std::pair< TreeIterator< Key, T >, bool > RedBlackTree< Key, T, Compare >::erase(const Key & key)
  {
    detail::Node< Key, T > * node = root_;
    detail::Node< Key, T > * prev = root_;
    detail::Node< Key, T > * to_delete = nullptr;
    while (!to_delete && node)
    {
      prev = node;
      if (node->data_.first == key)
      {
        to_delete = node;
      }
      else if (cmp_(key, node->data_.first))
      {
        node = node->left_;
      }
      else
      {
        node = node->right_;
      }
    }
    node = prev;
    if (!to_delete)
    {
      return std::make_pair(end(), false);
    }
    detail::Node< Key, T > * found = find_to_change_erased(to_delete);
    if (to_delete == root_ && !found)
    {
      clear();
      return std::make_pair(end(), true);
    }
    else if (!found)
    {
      found = to_delete;
    }
    if (found->parent_->left_ == found)
    {
      found->parent_->left_ = found->left_;
    }
    else if (found->parent_->right_ == found)
    {
      found->parent_->right_ = found->right_;
    }
    if (found != to_delete)
    {
      std::swap(found->data_, to_delete->data_);
    }
    auto iter = iterator(to_delete);
    delete found;
    return std::make_pair(++iter, true);
  }

  template< class Key, class T, class Compare >
  bool RedBlackTree< Key, T, Compare >::empty() const noexcept
  {
    return (!root_);
  }

  template< class Key, class T, class Compare >
  void RedBlackTree< Key, T, Compare >::swap(RedBlackTree< Key, T, Compare > & rhs)
  {
    std::swap(root_, rhs.root_);
  }

  template< class Key, class T, class Compare >
  void RedBlackTree< Key, T, Compare >::clear_subtree(detail::Node< Key, T > * subtree)
  {
    if (!subtree)
    {
      delete subtree;
      return;
    }
    clear_subtree(subtree->left_);
    clear_subtree(subtree->right_);
    delete subtree;
  }

  template< class Key, class T, class Compare >
  void RedBlackTree< Key, T, Compare >::insert_balance_case1(detail::Node< Key, T > * subtree)
  {
    if (subtree == root_)
    {
      subtree->color_ = detail::color_t::BLACK;
    }
    else
    {
      insert_balance_case2(subtree);
    }
  }

  template< class Key, class T, class Compare >
  void RedBlackTree< Key, T, Compare >::insert_balance_case2(detail::Node< Key, T > * subtree)
  {
    if (subtree->parent_->color_ == detail::color_t::RED)
    {
      insert_balance_case3(subtree);
    }
  }

  template< class Key, class T, class Compare >
  void RedBlackTree< Key, T, Compare >::insert_balance_case3(detail::Node< Key, T > * subtree)
  {
    detail::Node< Key, T > * uncle = find_uncle(subtree);
    if (uncle && uncle->color_ == detail::color_t::RED)
    {
      subtree->parent_->color_ = detail::color_t::BLACK;
      uncle->color_ = detail::color_t::BLACK;
      detail::Node< Key, T > * grand = find_grandparent(subtree);
      grand->color_ = detail::color_t::RED;
      insert_balance_case1(grand);
    }
    else
    {
      insert_balance_case4(subtree);
    }
  }

  template< class Key, class T, class Compare >
  void RedBlackTree< Key, T, Compare >::insert_balance_case4(detail::Node< Key, T > * subtree)
  {
    detail::Node< Key, T > * grand = find_grandparent(subtree);
    if (subtree == subtree->parent_->right_ && subtree->parent_ == grand->left_)
    {
      rotate_left(subtree->parent_);
      subtree = subtree->left_;
    }
    else if (subtree == subtree->parent_->left_ && subtree->parent_ == grand->right_)
    {
      rotate_right(subtree->parent_);
      subtree = subtree->right_;
    }
    insert_balance_case5(subtree);
  }

  template< class Key, class T, class Compare >
  void RedBlackTree< Key, T, Compare >::insert_balance_case5(detail::Node< Key, T > * subtree)
  {
    detail::Node< Key, T > * grand = find_grandparent(subtree);
    subtree->parent_->color_ = detail::color_t::BLACK;
    grand->color_ = detail::color_t::RED;
    if (subtree == subtree->parent_->left_ && subtree->parent_ == grand->left_)
    {
      rotate_right(grand);
    }
    else
    {
      rotate_left(grand);
    }
  }

  template< class Key, class T, class Compare >
  void RedBlackTree< Key, T, Compare >::erase_balance_case1(detail::Node< Key, T > * subtree)
  {
    return;
  }

  template< class Key, class T, class Compare >
  detail::Node< Key, T > * RedBlackTree< Key, T, Compare >::find_to_change_erased(detail::Node< Key, T > * subtree)
  {
    if (subtree->left_)
    {
      subtree = subtree->left_;
      while (subtree->right_)
      {
        subtree = subtree->right_;
      }
    }
    else if (subtree->right_)
    {
      subtree = subtree->right_;
      while (subtree->left_)
      {
        subtree = subtree->left_;
      }
    }
    else
    {
      subtree = nullptr;
    }
    return subtree;
  }

  template< class Key, class T, class Compare >
  detail::Node< Key, T > * RedBlackTree< Key, T, Compare >::find_grandparent(detail::Node< Key, T > * subtree)
  {
    if (subtree && subtree->parent_)
    {
      return subtree->parent_->parent_;
    }
    else
    {
      return nullptr;
    }
  }

  template< class Key, class T, class Compare >
  detail::Node< Key, T > * RedBlackTree< Key, T, Compare >::find_uncle(detail::Node< Key, T > * subtree)
  {
    detail::Node< Key, T > * node = find_grandparent(subtree);
    if (!node)
    {
      return nullptr;
    }
    if (node->parent_ == node->left_)
    {
      return node->right_;
    }
    else
    {
      return node->left_;
    }
  }

  template< class Key, class T, class Compare >
  detail::Node< Key, T > * RedBlackTree< Key, T, Compare >::find_brother(detail::Node< Key, T > * subtree)
  {
    if (subtree == subtree->parent_->left_)
    {
      return subtree->parent_->right_;
    }
    else
    {
      return subtree->parent_->left_;
    }
  }

  template< class Key, class T, class Compare >
  void RedBlackTree< Key, T, Compare >::rotate_left(detail::Node< Key, T > * subtree)
  {
    detail::Node< Key, T > * node = subtree->right_;
    node->parent_ = subtree->parent_;
    if (!node->parent_)
    {
      root_ = node;
    }
    if (subtree->parent_)
    {
      if (subtree->parent_->left_ == subtree)
      {
        subtree->parent_->left_ = node;
      }
      else
      {
        subtree->parent_->right_ = node;
      }
    }
    subtree->right_ = node->left_;
    if (node->left_)
    {
      node->left_->parent_ = subtree;
    }
    subtree->parent_ = node;
    node->left_ = subtree;
  }

  template< class Key, class T, class Compare >
  void RedBlackTree< Key, T, Compare >::rotate_right(detail::Node< Key, T > * subtree)
  {
    detail::Node< Key, T > * node = subtree->left_;
    node->parent_ = subtree->parent_;
    if (!node->parent_)
    {
      root_ = node;
    }
    if (subtree->parent_)
    {
      if (subtree->parent_->left_ == subtree)
      {
        subtree->parent_->left_ = node;
      }
      else
      {
        subtree->parent_->right_ = node;
      }
    }
    subtree->left_ = node->right_;
    if (node->right_)
    {
      node->right_->parent_ = subtree;
    }
    subtree->parent_ = node;
    node->right_ = subtree;
  }

  template< class Key, class T, class Compare >
  bool RedBlackTree< Key, T, Compare >::is_leaf(detail::Node< Key, T > * node)
  {
    return (node->left_ && node->right_);
  }
}

#endif
