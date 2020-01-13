#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <strings.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

#define SERV_PORT 5005
#define SERV_IP_ADDR "192.168.3.113"
#define BACKLOG 5
#define QUIT "quit"

int main(int argc, char* argv[]) {
    // 1.创建套接字
    int sockfd = -1;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == sockfd) {
        perror("socket");
        exit(1);
    }

    //优化1允许地址快速重用
    int b_reuse = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &b_reuse, sizeof(int));

    // 2.填充结构体
    struct sockaddr_in cin;
    bzero(&cin, sizeof(cin));
    socklen_t cinlen = sizeof(cin);
    struct sockaddr_in sin = {
        .sin_family = AF_INET,
        .sin_port = htons(SERV_PORT),  //本地字节序-》网络字节序
        .sin_addr.s_addr = htonl(INADDR_ANY),  //让服务器程序能绑定任意ip上
    };

    if (bind(sockfd, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
        perror("bind");
        exit(1);
    }
    if (listen(sockfd, 5) < 0) {
        perror("listen");
        exit(1);
    }

    int acceptfd;
    char buf[100] = {};

    //创建集合
    fd_set readfds, temp;
    FD_ZERO(&readfds);
    FD_ZERO(&temp);
    //将关心的文件描述符添加进去
    FD_SET(sockfd, &readfds);
    FD_SET(0, &readfds);

    int maxfd = sockfd;
    int val = -1;
    int i = 0;
    ssize_t recv_ty = 0;
    while (1) {
        temp = readfds;
        val = select(maxfd + 1, &temp, NULL, NULL, NULL);
        if (val < 0) {
            perror("select");
            exit(1);
        }

        // 0 1 2 3 4 5 6
        //轮训判断那个文件描述符产生事件
        for (i = 0; i < maxfd + 1; i++) {
            if (FD_ISSET(i, &temp)) {
                if (0 == i)  //输入
                {
                    bzero(buf, sizeof(buf));
                    fgets(buf, sizeof(buf), stdin);
                    if (!(strncasecmp(buf, QUIT, 4))) {
                        puts("服务器退出！");
                        exit(1);
                    }
                } else if (sockfd == i) {
                    // acceptfd = accept();
                    acceptfd = accept(sockfd, (struct sockaddr*)&cin, &cinlen);
                    if (acceptfd < 0) {
                        perror("accept");
                        exit(1);
                    }
                    printf("客户端 %s:%d已连接 fd=%d\n",
                           (char*)inet_ntoa(cin.sin_addr), ntohs(cin.sin_port),
                           acceptfd);
                    //把acceptfd 加入到readfds
                    FD_SET(acceptfd, &readfds);
                    //更新maxfd,判断最大的是那个赋值给maxfd
                    maxfd = (maxfd > acceptfd) ? maxfd : acceptfd;
                } else  //剩下的文件描述符就是 acceptfd们 用于通讯的
                {
                    bzero(buf, sizeof(buf));
                    //收消息
                    recv_ty = recv(i, buf, sizeof(buf), 0);
                    if (recv_ty < 0) {
                        perror("recv");
                        FD_CLR(i, &readfds);
                        close(i);
                    } else if (0 == recv_ty) {
                        printf("客户端fd=%d已经关闭!\n", i);
                        FD_CLR(i, &readfds);
                        close(i);
                    } else {
                        printf("recv:%s\n", buf);
                    }
                }
            }
        }
    }
    close(sockfd);
    return 0;
}
