#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#if 0  //其他用户
#define SERV_PORT 5020
#define SERV_IP_ADDR "192.168.3.116"
#else  //缺省设置
#define SERV_PORT 5001
#define SERV_IP_ADDR "192.168.3.113"
#endif

#define BACKLOG 5

int fd = 0;
int ret = 0;
int newfd = 0;
int t_bind = 0;

char buf[1024] = {};
char bufrecv[1024] = {};

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
#if 0
    if (connect(fd, (struct sockaddr*)&sin, sizeof(sin))) {
        perror("Connect");
        return 0;
    }  //绑定服务器
#else
    while (connect(fd, (struct sockaddr*)&sin, sizeof(sin))) {
        perror("Connect");
    }  //滞留等待服务器
#endif

    puts("客户端就绪!");

    while (1) {
        bzero(buf, 1024);

        if (!fgets(buf, 1024 - 1, stdin)) {
            continue;
        }  //写入

        if (send(fd, buf, 1024 - 1, 0) < 0) {
            perror("Send");
            exit(-1);
        }  //发送
#if 0
        bzero(bufrecv, 1024);
        ret = read(fd, bufrecv, 1024 - 1);
        if (ret < 0) {
            perror("");
            exit(1);
        }
        if (!ret)
            break;
        printf("Recv:%s\n", bufrecv);
#endif
    }

    close(fd);

    return 0;
}
