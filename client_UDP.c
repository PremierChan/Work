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

	int port = SERV_PORT;
	port = atoi(argv[2]);
	if((port<5001)&&(port>65535))
	{
		puts("端口非法！");
		exit(1);
	}

	//创建套接字
	int fd=-1;
	if((fd=socket(AF_INET,SOCK_DGRAM,0))<0)
	{
		perror("socket.");
		exit(1);
	}

	//填充服务器要绑定的信息结构体
	struct sockaddr_in sin;
	bzero(&sin,sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port   = htons(port);
	//让服务器能够绑定任意IP
	sin.sin_addr.s_addr=inet_addr(argv[1]);

	puts("客户端就绪");

	char buf[BUFSIZ];
	while(1)
	{
	
		bzero(buf,BUFSIZ);
		if(fgets(buf,BUFSIZ-1,stdin)==NULL)
		{
			perror("fgets");
			continue;
		}
		sendto(fd,buf,strlen(buf),0,(struct sockaddr*)&sin,sizeof(sin));
		
		if(!strncasecmp(buf,QUIT,4))
		{
			printf("我已退出\n");
			break;
		}
	}
	close(fd);
	return 0;
}

