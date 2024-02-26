#ifndef LISTITERATOT_HPP
#define LISTITERATOR_HPP
#include <memory>
#include <iterator>
#include "node.hpp"

namespace piyavkin
{
  template< class D >
  class ListIterator: public std::iterator< std::bidirectional_iterator_tag, D >
  {
  public:
    ListIterator():
      node(nullptr)
    {}
    ListIterator(Node< D >* nd):
      node(nd)
    {}
    ListIterator(const ListIterator< D >&) = default;
    ListIterator< D >& operator=(const ListIterator< D >&) = default;
    ~ListIterator() = default;
    ListIterator< D >& operator++()
    {
      node = node->next_;
      return *this;
    }
    ListIterator< D >& operator--()
    {
      node = node->prev_;
      return *this;
    }
    ListIterator< D > operator++(int)
    {
      ListIterator< D > result(*this);
      ++(*this);
      return result;
    }
    ListIterator< D > operator--(int)
    {
      ListIterator< D > result(*this);
      --(*this);
      return result;
    }
    bool operator==(const ListIterator< D >& rhs) const
    {
      return node == rhs.node;
    }
    bool operator!=(const ListIterator< D >& rhs) const
    {
      return !(*this == rhs);
    }
    D* operator->() const
    {
      return std::addressof(node->value_);
    }
    D& operator*() const
    {
      return node->value_;
    }
  private:
    Node< D >* node;
  };
}
#endif
