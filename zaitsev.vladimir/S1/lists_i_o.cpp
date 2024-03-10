#include "lists_i_o.hpp"
#include <iostream>
#include <string>
#include <cstring>
#include <stdexcept>
#include <limits>
#include "forward_list.hpp"

namespace zaitsev
{
  void input_lists(ForwardList< std::pair< std::string, ForwardList< ull > > >& list)
  {
    using List_t = ForwardList< unsigned long long >;
    std::string input;
    std::cin >> input;
    while (std::cin)
    {
      list.push_front({ input, List_t() });
      std::cin >> input;
      while (std::cin && !std::isalpha(input[0]))
      {
        list.front().second.push_front(std::stoull(input));
        std::cin >> input;
      }
    }
    if (list.empty())
    {
      std::cout << "0\n";
      throw std::underflow_error("No lists");
    }
  }

  ForwardList< unsigned long long >* print_lists(ForwardList< std::pair< std::string, ForwardList< ull > > >& list)
  {
    using ullList = ForwardList< ull >;
    using ullListIt = ForwardListIterator< ull >;

    ForwardList< std::pair< ullListIt, ullListIt > > beg_end_it;
    for (ForwardListIterator< std::pair < std::string, ullList > > i = list.begin(); i != list.end(); ++i)
    {
      i->second.reverse();
      if (i != list.begin())
      {
        std::cout << " ";
      }
      std::cout << i->first;
      beg_end_it.push_front({ i->second.begin(), i->second.end() });
    }
    beg_end_it.reverse();
    std::cout << '\n';

    ullList* sums = new ullList();
    ull sum = 1;
    bool overflow = false;
    while (sum)
    {
      sum = 0;
      size_t elements_nmb = 0;
      for (ForwardListIterator< std::pair< ullListIt, ullListIt > > i = beg_end_it.begin(); i != beg_end_it.end(); ++i)
      {
        if (i->first != i->second)
        {
          std::cout << (elements_nmb++ ? " " : "") << *(i->first);
          if (std::numeric_limits< ull >::max() - sum >= *(i->first) && !overflow)
          {
            sum += *(i->first);
          }
          else
          {
            overflow = true;
          }
          ++(i->first);
        }
      }
      if (sum)
      {
        std::cout << "\n";
        sums->push_front(sum);
      }
    }
    if (overflow)
    {
      delete sums;
      sums = nullptr;
    }
    return sums;
  }

  void print_sums(ForwardList< ull >& sums)
  {
    if (sums.empty())
    {
      std::cout << "0\n";
      return;
    }
    sums.reverse();
    auto i = sums.begin();
    std::cout << *(i++);
    for (; i != sums.end(); ++i)
    {
      std::cout << " " << *i;
    }
    std::cout << "\n";
  }
}
