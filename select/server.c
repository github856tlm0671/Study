#include <argp.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>

/* select整体逻辑：首先创建一个监听套接字，并绑定相应的地址端口信息，开始监听；然后将监听文件描述符添加到文件描述符集合中；
    一旦有客户端发起连接，则监听文件描述符缓存区产生数据，select函数解除阻塞返回，得到内核传出的标志位为1的文件描述符集合；
    然后调用accept函数，建立与客户端的通信同时返回通信套接字，并将通信文件描述符加入到文件描述符集合中，再对集合进行循环监听。*/

int main() {
    // 1. 创建监听的fd
    int lfd = socket(AF_INET, SOCK_STREAM, 0);

    // 2. 绑定
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind(lfd, (struct sockaddr*)&addr, sizeof(addr));

    // 3. 设置监听
    listen(lfd, 128);

    // 将监听的fd的状态检测委托给内核检测
    int maxfd = lfd;
    // 初始化检测的读集合
    fd_set rdset;  // 用于select函数的主集合
    fd_set rdtemp; // 用于每次调用select时的临时集合

    // 清零
    FD_ZERO(&rdset);
    // 将监听的lfd设置到检测的读集合中
    FD_SET(lfd, &rdset);
    // 通过select委托内核检测读集合中的文件描述符状态, 检测read缓冲区有没有数据
    // 如果有数据, select解除阻塞返回
    // 应该让内核持续检测
    while (1) {
        // 默认阻塞
        // 将rdset赋值给rdtemp，这样select只是修改传入的临时集合，
        rdtemp = rdset;

        // 只检测读缓冲区的文件描述符
        // select返回集合中已经就绪的文件描述符的总个数
        int num = select(maxfd + 1, &rdtemp, NULL, NULL, NULL);

        // rdset中的数据被内核改写了, 只保留了发生变化的文件描述的标志位上的1,
        // 没变化的改为0 只要rdset中的fd对应的标志位为1 -> 缓冲区有数据了

        // *********************判断有没有新连接，若有新连接，则将通信文件描述符添加到set集合*********************
        if (FD_ISSET(lfd, &rdtemp)) {
            // 接受连接请求, 这个调用不阻塞
            struct sockaddr_in cliaddr;
            socklen_t cliLen = sizeof(cliaddr);
            int cfd = accept(lfd, (struct sockaddr*)&cliaddr, &cliLen);

            // 得到建立连接后的 通信文件描述符
            // 将通信的文件描述符添加到读集合
            // 在下一轮select检测的时候, 就能得到通信文件描述符的缓冲区状态
            FD_SET(cfd, &rdset);
            // 重置最大的文件描述符
            maxfd = cfd > maxfd ? cfd : maxfd;
        }

        // *********************若没有新连接,
        // 则set内的通信文件描述符对应的线程开始通信*********************
        for (int i = 0; i < maxfd + 1; ++i) {
            // 判断从监听的文件描述符之后到maxfd这个范围内的文件描述符是否读缓冲区有数据
            if (i != lfd && FD_ISSET(i, &rdtemp)) {
                // 传入的i是通信文件描述符
                // 接收数据
                char buf[10] = {0};

                // 一次只能接收10个字节, 客户端一次发送100个字节
                // 一次是接收不完的, 文件描述符对应的读缓冲区中还有数据
                // 下一轮select检测的时候,
                // 内核还会标记这个文件描述符缓冲区有数据 -> 再读一次
                // 循环会一直持续, 知道缓冲区数据被读完位置
                int len = read(i, buf, sizeof(buf));
                if (len == 0) {
                    printf("客户端关闭了连接...\n");
                    // 将检测的文件描述符从读集合中删除
                    FD_CLR(i, &rdset);
                    close(i);
                } else if (len > 0) {
                    // 收到了数据
                    // 发送数据
                    write(i, buf, strlen(buf) + 1);
                } else {
                    // 异常
                    perror("read");
                }
            }
        }
    }

    return 0;
}