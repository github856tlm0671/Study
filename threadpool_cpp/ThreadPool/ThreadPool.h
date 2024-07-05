#ifndef _THREADPOOL_H 
#define _THREADPOOL_H 

#include <pthread.h>
#include <stdlib.h>
#include <iostream>
#include <queue>
#include <unistd.h>
#include "TaskQueue.h"

class ThreadPool
{
public:
    ThreadPool(int min, int max);
    ~ThreadPool();

    // 添加任务
    void addTask(Task t);
    // 获取忙的线程个数
    int getBusyNumber();
    // 获取存活的线程个数
    int getAliveNumber();

private:
    // 工作线程的任务函数
    static void* worker(void* arg);
    // 管理者线程的工作函数
    static void* manager(void* arg);
    // 线程退出函数
    void threadExit();


private:
    pthread_mutex_t m_lock;
    pthread_cond_t m_notEmpty;
    pthread_t* m_threadIDs;
    pthread_t m_managerID;
    TaskQueue* m_taskQ; 
    int m_maxNum;
    int m_minNum;
    int m_aliveNum;
    int m_busyNum;
    int m_exitNum;
    bool m_shutdown = false;
};

#endif