#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <strings.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

#define SERV_PORT 5001
#define SERV_IP_ADDR "192.168.3.118"
#define BACKLOG 5
#define QUIT "quit"




int main(int argc, char *argv[])
{
	//创建套接字
	int fd=-1;
	if((fd=socket(AF_INET,SOCK_DGRAM,0))<0)
	{
		perror("socket.");
		exit(1);
	}

	//允许绑定地址快速重用
	int b_reuse =1;
	setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&b_reuse,sizeof(b_reuse));

	//填充服务器要绑定的信息结构体
	struct sockaddr_in sin;
	bzero(&sin,sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port   = htons(SERV_PORT);
	//让服务器能够绑定任意IP
	sin.sin_addr.s_addr=htonl(INADDR_ANY);

	//绑定信息
	if(bind(fd,(struct sockaddr*)&sin,sizeof(sin))<0){
		perror("bind");
		exit(1);
	}

	puts("服务器就绪");

	char buf[BUFSIZ];
	struct sockaddr_in cin;
	socklen_t addrlen = sizeof(cin);
	while(1)
	{
		bzero(buf,BUFSIZ);
		if(recvfrom(fd,buf,BUFSIZ-1,0,(struct sockaddr*)&cin,&addrlen)<0)
		{
			perror("recvfrom");
			continue;
		}
		char ip[16];
		if(!inet_ntop(AF_INET,(void*)&cin.sin_addr.s_addr,ip,sizeof(cin)))
		{
			perror("inet_ntop.");
			continue;
		}
		
		printf("由%s:%d发送:%s\n",ip,ntohs(cin.sin_port),buf);
		
		if(!strncasecmp(buf,QUIT,4))
			printf("注意：%s:%d已退出\n",ip,ntohs(cin.sin_port));
	}
	close(fd);
	return 0;
}

