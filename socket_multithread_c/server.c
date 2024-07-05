#include <argp.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// 通信函数的传入参数
struct SockInfo {
    int fd;                  // 通信套接字
    pthread_t tid;           // 子线程ID
    struct sockaddr_in addr; // IP地址
};

// 子线程工作函数的参数数组
struct SockInfo infos[128];

// 通信工程函数
void* working(void* arg) {
    // 和客户端通信
    while (1) {
        struct SockInfo* info = (struct SockInfo*)arg;

        // 接受数据
        char buf[1024];
        int len = read(info->fd, buf, sizeof(buf));
        if (len == -1) {
            perror("read");
            printf("接收数据失败……\n");
            info->fd = -1;
            break;
        } else if (len == 0) {
            printf("客户端断开连接...\n");
            info->fd = -1;
            break;
        } else {
            write(info->fd, buf, strlen(buf) + 1);
        }
    }
    return NULL;
}

int check_fd_validity(int fd) {
    return fcntl(fd, F_GETFD) != -1 || errno != EBADF;
}

int main() {
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

    // 4.等待，接受连接请求
    // 数据初始化
    int max = sizeof(infos) / sizeof(infos[0]);
    for (int i = 0; i < max; i++) {
        bzero(&infos[i], sizeof(infos[i]));
        infos[i].fd = -1;
        infos[i].tid = -1;
    }

    int len = sizeof(struct sockaddr);
    // 父进程监听，子线程通信
    while (1) {
        struct SockInfo* pinfo;

        // 寻找工作函数参数数组中空数组，找到后就开始通信
        for (int i = 0; i < max; i++) {
            if (infos[i].fd == -1) {
                pinfo = &infos[i];
                break;
            }
            if (infos[i].tid == -1) {
                sleep(1);
                i--;
            }
        }

        // 在pinfo中写入IP地址参数
        int connfd = accept(fd, (struct sockaddr*)&pinfo->addr, &len);
        printf("parent thread, connfd: %d\n", connfd);
        if (connfd == -1) {
            perror("accept");
            exit(0);
        }

        // 在pinfo中写入通信套接字参数
        pinfo->fd = connfd;
        if (connfd == -1) {
            perror("accept");
            exit(0);
        }
        printf("fd的值是：%d\n", pinfo->fd);

        // 创建通信子线程，并将线程ID传入pinfo
        pthread_create(&pinfo->tid, NULL, working, pinfo);
        char ip[24] = {0};
        printf("客户端IP地址是：%s\n",
               inet_ntop(AF_INET, &(pinfo->addr), ip, sizeof(ip)));

        // 分离主线程和子线程
        pthread_detach(pinfo->tid);
    }

    // 释放资源
    close(fd); // 释放监听资源
    return 0;
}
