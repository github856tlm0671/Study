#include "threadpool.h"


// 线程池创建函数
ThreadPool* threadPoolCreate(int min, int max, int queueSize)
{
    // 为线程池开辟一块堆内存
    ThreadPool* pool = (ThreadPool*)malloc(sizeof(ThreadPool));
    do
    {
        // 判断线程池是否开辟成功
        if (pool == NULL)
        {
            printf("malloc threadpool fail……\n");
            break;
        }

        // 为线程池中的工作者线程单独再开辟一块堆区内存
        pool->threadIDs = (pthread_t*)malloc(sizeof(pthread_t) * max);
        if ((pool->threadIDs ==NULL))
        {
            printf("malloc threadIDs fail……\n");
            break;
        }

        // 将数组中的所有元素都初始化为0
        memset(pool->threadIDs, 0, sizeof(pthread_t)*max);

        pool->minNum = min;
        pool->maxNum = max;
        pool->busyNum = 0;
        pool->liveNum = min;    // 和线程池中最小的个数相等
        pool->exitnum = 0;

        if (pthread_mutex_init(&pool->mutexPool, NULL) != 0 ||
            pthread_mutex_init(&pool->mutexBusy, NULL) != 0 ||
            pthread_cond_init(&pool->notEmpty, NULL) != 0 ||
            pthread_cond_init(&pool->notFull, NULL) != 0)
        {
            printf("mutex or condition init fail……\n");
            break;
        }

        // 任务队列
        pool->taskQ = (Task*)malloc(sizeof(Task) * queueSize);
        pool->queueCapacity = queueSize;
        pool->queueSize = 0;
        pool->queueFront = 0;
        pool->queueRear = 0;
        
        pool->shutdown = 0;

        // 创建管理者线程，同时指定管理者函数
        pthread_create(&pool->managerID, NULL, manager, pool);
        
        // 创建工作者线程，同时指定工作者函数
        for(int i = 0; i < min; i++){
            pthread_create(&pool->threadIDs[i], NULL, woker, pool);
        }
        return pool;     
        
    } while (0);

    // 释放资源
    if (pool && pool->threadIDs) free(pool->threadIDs);
    if (pool && pool->taskQ) free(pool->taskQ);
    if (pool) free(pool);

    return NULL;
};

// 线程池销毁函数
int threadPoolDestory(ThreadPool* pool)
{
    if (pool ==NULL)
    {
        return -1;
    }

    // 关闭线程池
    pool->shutdown = 1;

    // 阻塞回收管理者线程，等待线程结束才会回收
    pthread_join(pool->managerID, NULL);

    // 唤醒阻塞在任务空的条件变量下的消费者线程
    for (int i = 0; i < pool->liveNum; i++)
    {
        pthread_cond_signal(&pool->notEmpty);
    }
    
    // 释放内存
    if (pool->taskQ)
    {
        free(pool->taskQ);
    }
    if(pool->threadIDs)
    {
        free(pool->threadIDs);
    }

    pthread_mutex_destroy(&pool->mutexBusy);
    pthread_mutex_destroy(&pool->mutexPool);
    pthread_cond_destroy(&pool->notEmpty);
    pthread_cond_destroy(&pool->notFull);

    free(pool);
    pool = NULL;

    return 0; 
};

// 管理者函数
// 主要任务：
// 1.检测任务个数，判断是否需要创建更多线程
// 2.检测空闲线程数，判断是否需要销毁线程
void* manager(void* arg)
{
    ThreadPool* pool = (ThreadPool*)arg;
    // 管理者需要一直管理线程池
    while (!pool->shutdown)
    {
        // 每隔3秒检测一次
        sleep(3);

        // 取出线程池中任务的数量和当前线程的数量
        pthread_mutex_lock(&pool->mutexPool);
        int quequeSize = pool->queueSize;
        int liveNum = pool->liveNum;
        pthread_mutex_unlock(&pool->mutexPool);

        // 取出当前忙的线程数量
        pthread_mutex_lock(&pool->mutexBusy);
        int busyNum = pool->busyNum;
        pthread_mutex_unlock(&pool->mutexBusy);

        // 添加线程
        // 任务的的个数 > 存活的线程数  && 存活的线程数 < 最大的线程数
        if (quequeSize > liveNum && liveNum < pool->maxNum)
        {
            pthread_mutex_lock(&pool->mutexPool);
            
            int counter = 0;
            for (int i = 0; i < pool->maxNum && counter < NUMBER
                && liveNum < pool->liveNum; i++)
            {
                if(pool->threadIDs[i] == 0)
                {
                    // 创建任务处理线程
                    pthread_create(&pool->threadIDs[i], NULL, woker, pool);
                    counter++;
                    pool->liveNum++;
                }
            }  
            pthread_mutex_unlock(&pool->mutexPool);
        }

        // 销毁线程
        // 忙的线程个数 * 2 < 存活线程个数 && 存活的线程个数 > 最小的线程数
        if (busyNum * 2 < liveNum && liveNum > pool->minNum)
        {
            pthread_mutex_lock(&pool->mutexPool);
            pool->exitnum = NUMBER;
            pthread_mutex_unlock(&pool->mutexPool);
            
            // 让空闲的工作线程自杀
            for (int i = 0; i < NUMBER; i++)
            {
                pthread_cond_signal(&pool->notEmpty);
            }
        }          
    }
    return NULL;   
}


// 工作者函数，相当于消费者
void* woker(void* arg)
{
    ThreadPool* pool = (ThreadPool*)arg;

    while(1)
    {
        pthread_mutex_lock(&pool->mutexPool);
        // 判断当前任务队列是否为空
        while (pool->queueSize == 0 && !pool->shutdown)
        {
            // 若任务队列为空，则工作线程会被条件变量阻塞；若阻塞后，有新的任务产生，阻塞线程将被唤醒。
            // 当管理者线程认为需要销毁时，唤醒被阻塞的线程
            pthread_cond_wait(&pool->notEmpty, &pool->mutexPool);

            // 判断是不是要销毁线程
            if (pool->exitnum > 0)
            {
                pool->exitnum--;
                if (pool->liveNum > pool->minNum)
                {
                    pool->liveNum--;

                    // 工作线程被唤醒后，抢到线程池的互斥锁以后，在这里需要解锁
                    pthread_mutex_unlock(&pool->mutexPool);
                    // 当前线程退出
                    threadExit(pool);
                }
            }
        }
        
        // 判断线程池是否被关闭
        if (pool->shutdown)
        {
            pthread_mutex_unlock(&pool->mutexPool);
            threadExit(pool);
        }

        // 从任务队列中取出一个任务
        Task task;
        task.function = pool->taskQ[pool->queueFront].function;
        task.arg = pool->taskQ[pool->queueFront].arg;

        // 移动头节点
        pool->queueFront = (pool->queueFront + 1) % pool->queueCapacity;
        pool->queueSize--;

        // 解锁
        // 消费者消费以后，唤醒因队列满了而阻塞的生产者，即阻塞在notFull的生产者
        pthread_cond_signal(&pool->notFull);   
        pthread_mutex_unlock(&pool->mutexPool);

        printf("thread %ld start working……\n",pthread_self());
        pthread_mutex_lock(&pool->mutexBusy);
        pool->busyNum++;
        pthread_mutex_unlock(&pool->mutexBusy);
        task.function(task.arg);    // 执行任务
        free(task.arg);             // 释放任务参数
        task.arg = NULL;            // 避免任务参数指针悬空

        printf("thread %ld end working...\n", pthread_self());
        pthread_mutex_lock(&pool->mutexBusy);
        pool->busyNum--;
        pthread_mutex_unlock(&pool->mutexBusy);
    }
    return NULL;
}

// 线程池任务添加函数，即生产者
/*
    传入参数：线程池，任务函数，任务函数所需的任务参数
*/
void threadPoolAdd(ThreadPool* pool, void(*func)(void*), void* arg)
{
    pthread_mutex_lock(&pool->mutexPool);

    // 任务队列数量 == 任务队列容量 && 线程池没有被关闭 
    while (pool->queueSize == pool->queueCapacity && !pool->shutdown)
    {
        // 队列已经满了，所以需要阻塞生产者线程
        // 一旦消费者完成消费后，任务队列不满，生产者马上被唤醒
        pthread_cond_wait(&pool->notFull, &pool->mutexPool);
    }

    if (pool->shutdown)
    {
        pthread_mutex_unlock(&pool->mutexPool);
        return;
    }

    // 在队尾添加任务
    pool->taskQ[pool->queueRear].function = func;
    pool->taskQ[pool->queueRear].arg = arg;
    pool->queueRear = (pool->queueRear + 1) % pool->queueCapacity;
    pool->queueSize++;

    // 唤醒阻塞在notEmpty条件变量的消费者线程（任务队列空了，所以消费者线程被阻塞）
    pthread_cond_signal(&pool->notEmpty);    
    pthread_mutex_unlock(&pool->mutexPool);  
    return;
}

// 检测线程池中忙碌的线程个数
int threadPoolBusyNum(ThreadPool* pool)
{
    pthread_mutex_lock(&pool->mutexBusy);
    int busyNum = pool->busyNum;
    pthread_mutex_unlock(&pool->mutexBusy);
    return busyNum;
}

// 检测线程池中存活的线程个数
int threadPoolAliveNum(ThreadPool* pool)
{
    pthread_mutex_lock(&pool->mutexPool);
    int aliveNum = pool->liveNum;
    pthread_mutex_unlock(&pool->mutexPool);
    return aliveNum;
}

// 线程退出函数
void threadExit(ThreadPool* pool)
{
    pthread_t tid = pthread_self();
    for (int i = 0; i < pool->maxNum; i++)
    {
        if (pool->threadIDs[i] == tid)
        {
            pool->threadIDs[i] = 0;
            printf("threadExit() called, %ld exiting...\n", tid);
            break;
        }
    }
    pthread_exit(NULL);
    return;
}