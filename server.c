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

#define SERV_PORT 5001
#define SERV_IP_ADDR "192.168.3.113"
#define BACKLOG 5

void child_fork(int);
void handler(int);

int fd = 0;
int ret = 0;
int newfd = 0;
int t_bind = 0;
int number = 0;



char buf[1024] = {};
char ipv4_addr[16] = {};

int main(int argc, char const* argv[]) {
    signal(SIGCHLD, handler);

    struct sockaddr_in sin;  //声明IP端口结构体
    struct sockaddr_in cin;

    socklen_t addrlen = sizeof(cin);

    bzero(&sin, sizeof(sin));  //将结构体置空

    sin.sin_family = AF_INET;         //选择协议
    sin.sin_port = htons(SERV_PORT);  //初始化端口
    //   sin.sin_addr.s_addr = htonl(INADDY_ANY);  //初始化IP地址
    sin.sin_addr.s_addr = inet_addr(SERV_IP_ADDR);

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
    puts("");

    while (1) {
        if ((newfd = accept(fd,(struct sockaddr*)&cin, &addrlen)) < 0) {
            perror("Accept");
            exit(-1);
        }  //开始接收阻塞态

        number += 1;

        if (!fork()) {
            if (!inet_ntop(AF_INET, (void*)&cin.sin_addr.s_addr, ipv4_addr,
                           sizeof(cin))) {
                perror("Inet Mtop");
                exit(1);
            }  //将IP和端口打印

            puts("===========================");
            printf("%d号登入\n客户端IP：%s \n端口号：%d\n", number, ipv4_addr,
                   ntohs(cin.sin_port));
            puts("===========================");
            puts("");

            child_fork(number);
        }

        close(newfd);
    }

    close(fd);
    close(newfd);
    //收尾
    return 0;
}

void child_fork(int number) {
    while (1) {
        if ((ret = read(newfd, buf, 1024 - 1)) < 0) {
            perror("Read");
            raise(SIGKILL);
        } else if (!ret) {
            break;
        }  //读取

        if (!strncasecmp(buf, "QUIT", 4)) {
            puts("-----------");
            printf("  %d号退出  \n", number);
            puts("-----------");
            puts("");

            raise(SIGKILL);

            break;
        }  //检测是否达成退出条件

        printf("%d号说:%s\n", number, buf);  //输出
    }
    raise(SIGKILL);
}

void handler(int number) {
    while (waitpid(-1, NULL, WNOHANG) > 0) {
    };  //循环收尸
}
