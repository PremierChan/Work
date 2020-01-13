#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

#define SERV_PORT 5001
#define SERV_IP_ADDR "192.168.3.113"
#define BACKLOG 5

void child_tid(st);

int fd = 0;
int ret = 0;
int newfd = 0;
int t_bind = 0;
int number = 0;

pthread_t tid = 0;

char buf[1024] = {};
char ipv4_addr[16] = {};

typedef struct tid_server {
    int fds;
    int number;
    socklen_t size;
    struct sockaddr_in cin;
} * st, st_size;

int main(int argc, char const* argv[]) {
    st st_cin;
    st_cin = (st*)malloc(sizeof(st_size));

    struct sockaddr_in sin;  //声明IP端口结构体
    //    struct sockaddr_in cin;
    //    socklen_t addrlen = sizeof(cin);

    st_cin->size = sizeof(st_cin->cin);

    bzero(&sin, sizeof(sin));  //将结构体置空

    sin.sin_family = AF_INET;         //选择协议
    sin.sin_port = htons(SERV_PORT);  //初始化端口
    //   sin.sin_addr.s_addr = htonl(INADDY_ANY);  //初始化IP地址
    sin.sin_addr.s_addr = inet_addr(SERV_IP_ADDR);

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket");
        exit(-1);
    }  //创建套接字

    if (bind(fd, (struct sockaddr*)&sin, st_cin->size)) {
        perror("Bind");
        exit(-1);
    }  //绑定套接字

    if (listen(fd, BACKLOG)) {
        perror("Listen");
        return 0;
    }  //转化为监听状态

    puts("服务器已就绪！");
    puts("");

    while (1) {
        if ((st_cin->fds = accept(fd, (struct sockaddr*)&(st_cin->cin),
                                  &(st_cin->size))) < 0) {
            perror("Accept");
            exit(-1);
        }  //开始接收阻塞态

        st_cin->number = (number + 1);

        pthread_create(&tid, NULL, &child_tid, st_cin);

        close(st_cin->fds);
    }

    close(fd);
    close(st_cin->fds);
    //收尾
    return 0;
}

void child_tid(st st_cin) {
    if (!inet_ntop(AF_INET, (void*)&(st_cin->cin.sin_addr.s_addr), ipv4_addr,
                   st_cin->size)) {
        perror("Inet Mtop");
        exit(1);
    }  //将IP和端口打印

    puts("===========================");
    printf("%d号登入\n客户端IP：%s \n端口号：%d\n", st_cin->number + 1,
           ipv4_addr, ntohs(st_cin->cin.sin_port));
    puts("===========================");
    puts("");

    while (1) {
        if ((ret = read(st_cin->fds, buf, 1024 - 1)) < 0) {
            perror("Read");
            //          raise(SIGKILL);
            pthread_exit("Quit");
        } else if (!ret) {
            break;
        }  //读取

        if (!strncasecmp(buf, "QUIT", 4)) {
            puts("-----------");
            printf("  %d号退出  \n", st_cin->number);
            puts("-----------");
            puts("");

            //            raise(SIGKILL);
            pthread_exit("Quit");

            break;
        }  //检测是否达成退出条件

        printf("%d号说:%s\n", st_cin->number, buf);  //输出
    }
    //    raise(SIGKILL);
    pthread_exit("Quit");
}
