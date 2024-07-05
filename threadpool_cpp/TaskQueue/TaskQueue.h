#ifndef _TASKQUEUE_H 
#define _TASKQUEUE_H 

#include <pthread.h>
#include <stdlib.h>
#include <iostream>
#include <queue>

// 定义了一个别名callback，它是一个函数指针类型，指向一个返回类型为void，参数为void* 的函数
using callback = void(*)(void*);

// 定义任务结构体
struct Task
{
    // 默认构造函数
    Task()
    {
        funcptr = nullptr;
        arg = nullptr;
    }
    // 参数化构造函数
    Task(callback f, void* a)
    {
        funcptr = f;
        this->arg = a;
    }

    // 成员函数指针
    callback funcptr;
    // 成员变量
    void* arg;
};

// 任务队列
class TaskQueue
{
public:
    TaskQueue();
    ~TaskQueue();

    // 添加任务（重载）
    void addTask(Task& task);
    void addTask(callback funcptr, void* arg);

    // 取出一个任务
    Task takeTask();

    // 获取当前任务队列中任务个数
    inline int taskNumber()
    {
        return m_queue.size();
    }

private:
    pthread_mutex_t m_mutex;    // 互斥锁
    std::queue<Task> m_queue;   // 任务队列
};

#endif