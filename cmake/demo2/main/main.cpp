#include "add.h"
#include "mydata.pb.h"
#include "sub.h"
#include <iostream>

int main() {
  int a, b;

  std::cout << "a = ";
  std::cin >> a;
  std::cout << "b = ";
  std::cin >> b;

  double res1, res2;
  res1 = add(a, b);
  std::cout << "a + b = " << res1 << std::endl;

  res2 = sub(a, b);

  ming::mydata data;
  data.set_num1(a);
  data.set_num2(b);
  data.set_res(res2);

  // 序列化
  std::string output;
  data.SerializeToString(&output);

  // 反序列化
  ming::mydata dd;
  dd.ParseFromString(output);
  std::cout << dd.num1() << " - " << dd.num2() << " = " << dd.res()
            << std::endl;
}