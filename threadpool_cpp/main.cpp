#include "ThreadPool.h"
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
using namespace std;

void taskFunc(void* arg)
{
    int num = *(int*)arg;
    cout << "thread: " << pthread_self() << ", number=" << num << endl;
    sleep(1);
}

int main()
{
	// 创建线程池
	ThreadPool pool(3,10);

    // 向线程池中添加任务
	for (int i = 0; i < 100; ++i) {
        int* num = new int(i+100);
        pool.addTask(Task(taskFunc, num));
    }
	sleep(20);
    return 0;
}


