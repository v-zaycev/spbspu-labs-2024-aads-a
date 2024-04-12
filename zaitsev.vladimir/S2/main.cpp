#include <iostream>
#include <string>
#include <fstream>
#include "token.hpp"
#include "infix_expression.hpp"
#include "postfix_expression.hpp"
#include "stack.hpp"
#include "queue.hpp"
#include "operators.hpp"

int main(int argc, char** argv)
{
  std::ifstream finput;
  if (argc == 2)
  {
    finput.open(argv[1]);
  }
  std::istream& input = argc == 2 ? finput : std::cin;
  using namespace zaitsev;
  try
  {
    Stack< long long >results;
    while (input)
    {
      std::string s;
      std::getline(input, s);
      if (s == "")
      {
        continue;
      }
      PostfixExpression< long long > post(InfixExpression< long long >(s, std::stoll));
      results.push(post.calculate());
    }
    std::cout << results.top();
    for (Stack< long long >::const_iterator i = ++results.cbegin(); i != results.cend(); ++i)
    {
      std::cout << ' ' << *i;
    }
    std::cout << '\n';
  }
  catch (const std::exception& e)
  {
    std::cerr <<"Error: "<< e.what() << '\n';
    return 1;
  }
  return 0;
}