#include <argp.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "threadpool.h"

// 通信函数的传入参数
typedef struct SockInfo {
    int fd;                  // 通信套接字
    pthread_t tid;           // 子线程ID
    struct sockaddr_in addr; // IP地址
} SockInfo;

// 线程池参数
typedef struct PoolInfo {
    ThreadPool* pool; // 线程池地址
    int fd;           // 监听文件符
} PoolInfo;

// 通信工程函数
void working(void* arg)
{
    SockInfo* pinInfo = (SockInfo*)arg;

    // 若连接成功，则打印客户端的IP地址和端口号
    char IP[32];
    printf("客户端的IP：%s， 端口号：%d",
           inet_ntop(AF_INET, &pinInfo->addr.sin_addr, IP, sizeof(IP)),
           ntohs(pinInfo->addr.sin_port));

    // 和客户端通信
    while (1) {
        // 接受数据
        char buf[1024];
        int len = read(pinInfo->fd, buf, sizeof(buf));
        if (len == -1) {
            perror("read");
            printf("接收数据失败……\n");
            pinInfo->fd = -1;
            break;
        } else if (len == 0) {
            printf("客户端断开连接...\n");
            pinInfo->fd = -1;
            break;
        } else {
            printf("客户端说：%s\n", buf);
            write(pinInfo->fd, buf, strlen(buf) + 1);
        }
    }
    close(pinInfo->fd);
}

void acceptConn(void* arg)
{
    PoolInfo* poolInfo = (PoolInfo*)arg;
    // 4.等待，接受连接请求
    int len = sizeof(struct sockaddr);
    // 父进程监听，子线程通信
    while (1) {
        struct SockInfo* pinfo;
        pinfo = (SockInfo*)malloc(sizeof(SockInfo));
        pinfo->fd = accept(poolInfo->fd, (struct sockaddr*)&pinfo->addr, &len);
        // 判断是否接受成功
        if (pinfo->fd == -1) {
            perror("accept");
            break;
        }
        // 向线程池添加通信任务
        threadPoolAdd(poolInfo->pool, working, pinfo);
    }
    // 释放资源
    close(poolInfo->fd); // 释放监听资源
}

int main()
{
    // 1.创建监听套接字
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("socket");
        exit(0);
    }

    // 2.将socket()返回值和本地IP端口绑定到一起
    // 初始化网络地址信息结构体，其中struct
    // sockaddr_in是IPV4，struct_sockaddr是IPV6
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;    // IPV4
    addr.sin_port = htons(10000); // 大端端口
    // INADDR_ANY代表本机的所有IP, 假设有三个网卡就有三个IP地址
    // 这个宏可以代表任意一个IP地址
    // 这个宏一般用于本地的绑定操作
    addr.sin_addr.s_addr = INADDR_ANY; // 本地IP
    int ret = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    if (ret == -1) {
        perror("bind");
        exit(0);
    }

    // 3.设置监听
    ret = listen(fd, 100);
    if (ret == -1) {
        perror("listen");
        exit(0);
    }

    // 创建线程池
    ThreadPool* pool = threadPoolCreate(3, 10, 100);
    PoolInfo* info = (PoolInfo*)malloc(sizeof(PoolInfo));
    info->pool = pool;
    info->fd = fd;
    // 向线程池添加监听任务
    threadPoolAdd(pool, acceptConn, info);
    pthread_exit(NULL);
    return 0;
}
