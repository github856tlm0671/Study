#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define NUMBER 2

// 任务结构体
typedef struct Task
{
    void(*function)(void *arg);
    void* arg;
}Task;

// 线程池结构体
typedef struct ThreadPool
{
    //任务队列
    Task* taskQ;
    int queueCapacity;      // 任务容量
    int queueSize;          // 当前任务个数
    int queueFront;         // 队头 -> 取数据
    int queueRear;          // 队尾 -> 放数据

    pthread_t managerID;    // 管理者线程ID
    pthread_t *threadIDs;   // 工作者线程ID
    int minNum;             // 最小的线程数
    int maxNum;             // 最大的线程数
    int busyNum;            // 忙碌线程个数
    int liveNum;            // 存活线程个数
    int exitnum;            // 需要销毁的线程个数

    pthread_mutex_t mutexPool;  // 锁整个线程池
    pthread_mutex_t mutexBusy;  // 锁busyNum变量
    pthread_cond_t notFull;     // 任务队列是不是满了
    pthread_cond_t notEmpty;    // 任务队列是不是空了

    int shutdown;            // 判断是否要销毁线程池，销毁为1，不销毁为0

}ThreadPool;

// 线程池创建函数
ThreadPool* threadPoolCreate(int min, int max, int queueSize);

// 线程池销毁函数
int threadPoolDestory(ThreadPool* pool);

// 检测线程池中存活的线程个数
int threadPoolAliveNum(ThreadPool* pool);

// 检测线程池中忙碌的线程个数
int threadPoolBusyNum(ThreadPool* pool);

// 线程池增加函数
void threadPoolAdd(ThreadPool* pool, void(*func)(void*), void* arg);

// 管理者函数
void* manager(void* arg);

// 工作者函数
void* woker(void* arg);

// 线程退出函数
void threadExit(ThreadPool* pool);


#endif