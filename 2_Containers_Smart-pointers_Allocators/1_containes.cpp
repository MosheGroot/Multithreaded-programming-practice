#include <iostream>
#include <string>
#include <regex>
#include <map>
#include <sstream>


std::string derivative(std::string polynomial) {
  std::map<int, int>  pow_coeff; //< pow to coeff map

  // parse regex  
  //              | + | [-]k        | x  |   pow    |
  std::regex  reg("[+]?([-]?[0-9]*)[\\*]?(x?)[\\^]?([0-9]*)");
  std::smatch m;
  while (std::regex_search(polynomial,  m, reg) && !polynomial.empty())
  {
    int k, pow;

    // coeff
    if (!m[1].str().empty())
    {
      if (m[1].length() == 1 && m[1] == '-')
        k = -1;
      else
        k = std::stoi(m[1].str());
    }
    else
      k = 1;

    // x
    if (!m[2].str().empty())
    {
      if (!m[3].str().empty())
        pow = std::stoi(m[3].str());
      else
        pow = 1;
    }
    else
      pow = 0;

    // insert to polynomial map
    pow_coeff[pow] += k;

    // get next polynominal part
    polynomial = m.suffix();
  }

  //get derivative
  std::stringstream buffer;
  for (auto it = pow_coeff.rbegin(); it != pow_coeff.rend(); ++it)
  {
    int pow = it->first;
    int k = it->second;

    if (!pow)             // x^0
      continue;
    
    k *= pow;
    pow -= 1;

    if (k >= 0)
      buffer << '+';
    
    if ((k != 1 && k != -1) || pow == 0)
      buffer << k;
    else if (k == -1)
      buffer << '-';

    if (pow)
      buffer<< "*x";

    if (pow > 1)
      buffer << '^' << pow;
  }

  // return
  std::string out = buffer.str();
  if (out.front() == '+')
    out.erase(0, 1);
  return out;
}


int main(void)
{
  std::string polynomial;
  std::getline(std::cin, polynomial);
  
  std::cout << derivative(polynomial) << std::endl;

  return 0;
}