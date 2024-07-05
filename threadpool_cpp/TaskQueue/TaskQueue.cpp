#include "TaskQueue.h"

// 构造函数
TaskQueue::TaskQueue()
{
    // 初始化互斥锁
    pthread_mutex_init(&m_mutex, NULL);
}

// 析构函数
TaskQueue::~TaskQueue()
{
    // 销毁互斥锁
    pthread_mutex_destroy(&m_mutex);
}

// 添加任务：参数为任务
void TaskQueue::addTask(Task &task)
{
    pthread_mutex_lock(&m_mutex);
    m_queue.push(task);
    pthread_mutex_unlock(&m_mutex);
}

// 重载添加任务：参数为回调函数指针，函数所需参数
void TaskQueue::addTask(callback funcptr, void *arg)
{
    pthread_mutex_lock(&m_mutex);
    Task task;
    task.funcptr = funcptr;
    task.arg = arg;
    m_queue.push(task);
    pthread_mutex_unlock(&m_mutex);
}

// 取出任务
Task TaskQueue::takeTask()
{
    Task t;
    pthread_mutex_lock(&m_mutex);

    // 判断任务队列是否为空
    if(m_queue.size() > 0)
    {
        t = m_queue.front();
        m_queue.pop();  
    }
    pthread_mutex_unlock(&m_mutex);
    return t;
}
