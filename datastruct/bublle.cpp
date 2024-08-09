#include <iostream>
using namespace std;

// 冒泡排序
void Bublle(int a[], int len) {
  int flag = 1;
  for (int i = 0; i <= len - 1; i++) {
    flag = 0;
    for (int j = 1; j <= len - 1; j++) {
      if (a[j] < a[j - 1]) {
        swap(a[j],a[j-1]);
      }
      flag = 1;
    }
  }
}

// 选择排序
void Select(int a[], int len){
  int minValue = 0;
    for (int i = 0; i < len; i++){
      minValue = a[i];
      for (int j = i+1; j < len; j++)
      {
        if (a[j] > a[i])
        {
          swap(a[i], a[j]);
        }
        
      }
    }  
}

// 插入排序

// 希尔排序


int main() {
  int a[6] = {10, 5, 6, 8, 2, 9};
  int len = 6;
  Bublle(a, len);
  for (int i = 0; i < len; i++) {
    cout << "第" << i << "个数是：" << a[i] << endl;
  }
}
