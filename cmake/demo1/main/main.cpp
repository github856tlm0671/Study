#include "add.h"
#include "sub.h"
#include <iostream>

int main() {
  int a, b;
  std::cout << "a = ";
  std::cin >> a;
  std::cout << "b = ";
  std::cin >> b;

  int res1, res2;
  std::cout << "a + b = " << add(a, b) << std::endl;
  std::cout << "b - b = " << sub(a, b) << std::endl;
}