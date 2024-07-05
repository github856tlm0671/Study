/*************************************************************************
	> File Name: ThreadPool.cpp
	> Author: Logan
	> Mail: wunaideshen@gmail.com 
	> Created Time: 2024年06月21日 星期五 23时06分55秒
 ************************************************************************/

#include <iostream>
#include <stdlib.h>
#include <cstring>
#include "ThreadPool.h"

ThreadPool::ThreadPool(int min, int max)
{
	// 实例化任务队列
	m_taskQ = new TaskQueue;

	do{
		// 初始化线程池
		m_minNum = min;
		m_maxNum = max;
		m_busyNum = 0;
		m_aliveNum = min;

		// 根据线程的最大上限给线程数组分配内存
		m_threadIDs = new pthread_t[max];

		if (m_threadIDs == nullptr)
		{
			std::cout << "malloc thread_t[] 失败……" << std::endl;
			break;
		}
		
		// 初始化
		std::memset(m_threadIDs, 0, sizeof(pthread_t)*max);
		// 初始化互斥锁，条件变量
		if (pthread_mutex_init(&m_lock, NULL) != 0 ||
			pthread_cond_init(&m_notEmpty, NULL) != 0)
		{
			std::cout << "init mutex or condition fail……" << std::endl;
			break;
		}
		
		/****************************创建线程*****************************/
		// 创建管理者线程
		pthread_create(&m_managerID, NULL, worker, this);
		// 创建工作者线程
		for (int i = 0; i < min; i++)
		{
			pthread_create(&m_threadIDs[i], NULL, worker, this);
			std::cout << "创建子线程,ID:" << std::to_string(m_threadIDs[i]) << std::endl;
		}
		
	}while(0);
}

ThreadPool::~ThreadPool()
{
	m_shutdown = true;
	// 销毁管理者线程
	pthread_join(m_managerID, NULL);
	// 销毁所有消费者线程
	for (int i = 0; i < m_aliveNum; i++)
	{
		pthread_cond_signal(&m_notEmpty);

	}
	if (m_taskQ) delete m_taskQ;
	if (m_threadIDs) delete[] m_threadIDs;
	pthread_mutex_destroy(&m_lock);
	pthread_cond_destroy(&m_notEmpty);	
}

void ThreadPool::addTask(Task t)
{
	if (m_shutdown)
	{
		return;
	}
	// 调用任务队列对像的任务添加函数，该函数内部加锁，因此不需要再次加锁
	m_taskQ->addTask(t);
	// 唤醒因任务队列空而阻塞的消费者
	pthread_cond_signal(&m_notEmpty);
}

int ThreadPool::getBusyNumber()
{
	int busyNumber;
	pthread_mutex_lock(&m_lock);
	busyNumber = m_busyNum;
	pthread_mutex_unlock(&m_lock);
    return busyNumber;
}

int ThreadPool::getAliveNumber()
{
    int aliveNumber;
	pthread_mutex_lock(&m_lock);
	aliveNumber = m_busyNum;
	pthread_mutex_unlock(&m_lock);
    return aliveNumber;
}

// 工作线程 消费者
void *ThreadPool::worker(void *arg)
{
	ThreadPool* pool = static_cast<ThreadPool*>(arg);
	// 不停的工作
	while (true)
	{
		// 访问任务队列(共享资源)加锁
		pthread_mutex_lock(&pool->m_lock);
		// 判断任务队列是否为空，若为空则工作线程阻塞
		while(pool->m_taskQ->taskNumber() ==0 && !pool->m_shutdown)
		{
			std::cout << "thread " << std::to_string(pthread_self()) << " waiting……\n" << std::endl; 

			// 阻塞线程
			pthread_cond_wait(&pool->m_notEmpty, &pool->m_lock);

			// 解除线程后，判断是否要销毁线程
			if (pool->m_exitNum > 0)
			{
				pool->m_exitNum--;
				if (pool->m_aliveNum > pool->m_minNum)
				{
					pool->m_aliveNum--;
					pthread_mutex_unlock(&pool->m_lock);
					pool->threadExit();
				}
			}			
		}

		// 判断线程池是否要关闭
		if (pool->m_shutdown)
		{
			pthread_mutex_unlock(&pool->m_lock);
			pool->threadExit();
		}

		// 从队列中取出一个任务
		Task task = pool->m_taskQ->takeTask();
		pool->m_busyNum++;
		// 线程池解锁
		pthread_mutex_unlock(&pool->m_lock);
		// 执行任务
		std::cout<< "thread " << std::to_string(pthread_self()) << " start working……" << std::endl;
		task.funcptr(task.arg);
		delete task.arg;
		task.arg = nullptr;

		// 任务处理结束
		std::cout<< "thread " << std::to_string(pthread_self()) << " end working……" << std::endl;
		pthread_mutex_lock(&pool->m_lock);
		pool->m_busyNum--;
		pthread_mutex_unlock(&pool->m_lock);
	}
	
    return nullptr;
}

void *ThreadPool::manager(void *arg)
{
	ThreadPool* pool = static_cast<ThreadPool*>(arg);

	// 如果线程没有关闭，就一直检测
	while (!pool->m_shutdown)
	{
		// 每隔5s检测一次
		sleep(5);
		// 取出线程池中的任务数和线程数
		// 取出工作的线程数
		pthread_mutex_lock(&pool->m_lock);
		int taskNum = pool->m_taskQ->taskNumber();
        int aliveNum = pool->m_aliveNum;
		int busyNum = pool->m_busyNum;
		pthread_mutex_unlock(&pool->m_lock);
		
		const int NUMBER = 2;
        // 如果任务数小于线程数，并且线程数小于线程的上限，创建新线程
        if (taskNum < aliveNum && aliveNum < pool->m_maxNum)
        {
            // 线程池加锁
			pthread_mutex_lock(&pool->m_lock);
            int num = 0;
			// 创建新线程
			for (int i = 0; i < pool->m_maxNum && num < NUMBER && pool->m_aliveNum < pool->m_maxNum; i++)
			{
				if (pool->m_threadIDs[i] == 0)
				{
					pthread_create(&pool->m_threadIDs[i], NULL, worker, pool);
					num++;
                    pool->m_aliveNum++;
				}
				
			}
			// 线程池解锁
			pthread_mutex_unlock(&pool->m_lock);
        }

		// 销毁多余的线程
		// 存活线程数 > 忙的线程数*2 && 存活线程数 > 最小线程数
		if (aliveNum > busyNum * 2 && aliveNum > pool->m_minNum)
        {
            // 线程池加锁
            pthread_mutex_lock(&pool->m_lock);
            pool->m_exitNum = NUMBER;
            pthread_mutex_unlock(&pool->m_lock);
			for (int i = 0; i < NUMBER; i++)
			{
				pthread_cond_signal(&pool->m_notEmpty);
			}
        }
	}
    return nullptr;
}

// 线程退出
void ThreadPool::threadExit()
{
	pthread_t tid = pthread_self();
	for (int i = 0; i < m_maxNum; i++)
	{
		if (m_threadIDs[i] == tid)
		{
			std::cout << "threadExit() function:thread " << std::to_string(tid) << " exit... " << std::endl;
			m_threadIDs[i] = 0;
			break;
		}
		
	}
	pthread_exit(NULL);
}
