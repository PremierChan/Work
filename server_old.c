#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>

#define SERV_PORT 5001
#define SERV_IP_ADDR "192.168.3.113"
#define BACKLOG 5

void read_fion(void);   //读函数
void write_fion(void);  //写函数

int fd = 0;
int newfd = 0;
int t_bind = 0;
int ret = 0;
char buf[1024] = {};
pthread_t tid1 = 0;
pthread_t tid2 = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char const* argv[]) {
    struct sockaddr_in sin;    //声明IP端口结构体
    bzero(&sin, sizeof(sin));  //将结构体置空

    sin.sin_family = AF_INET;                       //选择协议
    sin.sin_port = htons(SERV_PORT);                //初始化端口
    sin.sin_addr.s_addr = inet_addr(SERV_IP_ADDR);  //初始化IP地址

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket");
        exit(-1);
    }  //创建套接字

    if (bind(fd, (struct sockaddr*)&sin, sizeof(sin))) {
        perror("Bind");
        exit(-1);
    }  //绑定套接字

    if (listen(fd, BACKLOG)) {
        perror("Listen");
        return 0;
    }  //转化为监听状态

    puts("服务器已就绪！");

    pthread_mutex_init(&mutex, NULL);

    if ((newfd = accept(fd, NULL, NULL)) < 0) {
        perror("Accept");
        return 0;
    }  //开始接收阻塞态

#if 0
    if (pthread_create(&tid1, NULL, read_fion, NULL)) {
        perror("Pthread Create");
        exit(-1);
    }  //创建线程1

    if (pthread_create(&tid1, NULL, read_fion, NULL)) {
        perror("Pthread Create");
        exit(-1);
    }  //创建线程2

    if (!pthread_join(tid1, NULL)) {
        pthread_join(tid2, NULL);
    }
#endif

#if 1
    while (1) {
        if ((ret = read(newfd, buf, 1024 - 1)) < 0) {
            perror("Read");
            exit(-1);
        } else if (!ret) {
            break;
        }  //读取

        printf("Rec:%s\n", buf);  //输出

        if (!strncasecmp(buf, "QUIT", 4)) {
            puts("客户端退出!");
            break;
        }  //检测是否达成退出条件
#if 1
        if (!fgets(buf, 1024 - 1, stdin)) {
            continue;
        }  //输入

        write(newfd, buf, strlen(buf));  //写入
#endif
    }

    puts("服务器退出!");
    pthread_mutex_destroy(&mutex);

    close(fd);
    close(newfd);
    //收尾
    return 0;
}

#else

    void read_fion(void) {
        while (1) {
            pthread_mutex_lock(&mutex);
            puts("A");
            if ((ret = read(newfd, buf, 1024 - 1)) < 0) {
                perror("Read");
                exit(-1);
            } else if (!ret) {
                break;
            }  //读取

            printf("Rec:%s\n", buf);  //输出

            if (!strncasecmp(buf, "QUIT", 4)) {
                puts("客户端退出!");
                break;
            }  //检测是否达成退出条件
            pthread_mutex_unlock(&mutex);
        }
    }

    void write_fion(void) {
        while (1) {
            pthread_mutex_lock(&mutex);
            puts("B");
            if (!fgets(buf, 1024 - 1, stdin)) {
                continue;
            }  //输入

            write(newfd, buf, strlen(buf));  //写入
            pthread_mutex_unlock(&mutex);
        }
    }

#endif