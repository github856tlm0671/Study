/*************************************************************************
    > File Name: main.c
    > Author: ma6174
    > Mail: ma6174@163.com
    > Created Time: 2024年08月07日 星期三 21时12分20秒
 ************************************************************************/

#include "add/add.h"
#include "sub/sub.h"
#include <stdio.h>

int main() {

  int a = 10;
  int b = 5;
  int res1 = add(a, b);
  int res2 = sub(a, b);
  printf("%d\n", res1);
  printf("%d\n", res2);
}
