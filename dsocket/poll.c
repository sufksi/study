#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>
#define PORT 1234
void sys_err(char *str)
{
	perror(str);
	exit(-1);
}

int main()
{
	int lfd;
	int ret;
	struct pollfd cfds[BUFSIZ];
	struct sockaddr_in sAddr,cAddr;
	int maxi,num;
	char buf[BUFSIZ];
	char bufIp[BUFSIZ];
	socklen_t cAddrLen;
	sAddr.sin_family = AF_INET;
	sAddr.sin_port = htons(PORT);
	sAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	lfd = socket(AF_INET,SOCK_STREAM,0);
	if(lfd == -1)
		sys_err("socket failure!\n");
	
	int opt = 1;
	setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	
	ret = bind(lfd,(struct sockaddr *)&sAddr,sizeof(sAddr));
	if(-1 == ret)
		sys_err("bind failure!\n");
	
	ret = listen(lfd,128);
	if(-1 == ret)
		sys_err("listen failure!\n");
	
	for(int i = 0;i<BUFSIZ;i++)
	{
		cfds[i].fd = -1;
	}
	cfds[0].fd = lfd;
	cfds[0].events = POLLIN;
	maxi = 0;	
	
	while(1)
	{
		num = poll(cfds,maxi + 1,0);
		if(-1 == num)
			sys_err("poll failure!\n");
		
		if(cfds[0].revents & POLLIN)
		{
			int i;
			cAddrLen = sizeof(cAddr);
			ret = accept(cfds[0].fd,(struct sockaddr *)&cAddr,&cAddrLen);
			if(-1 == ret)
				sys_err("accept failure!\n");
			printf("[connected] IP = %s,fd = %d\n",inet_ntop(AF_INET,(void*)&cAddr.sin_addr.s_addr,bufIp,BUFSIZ),ret);
				
			for(i = 0;i<BUFSIZ;i++)
			{
				if(-1 == cfds[i].fd)
				{
					cfds[i].fd = ret;
					cfds[i].events = POLLIN;
					break;
				}
			}
			if(i > maxi)
				maxi = i;
			
			if(1 == num)
				continue;	
		}
		
		for(int i = 1;i<=maxi;i++)
		{
			if(cfds[i].fd == -1)
				continue;
			
			if(cfds[i].revents & POLLIN)
			{
				ret = read(cfds[i].fd,buf,BUFSIZ);
				if(-1 == ret)
					sys_err("read failure\n");
				if(0 == ret)
				{
					close(cfds[i].fd);
					cfds[i].fd = -1;
				}
				
				for(int j = 0;j<ret;j++)
				{
					buf[j] = toupper(buf[j]);
				}
				
				write(cfds[i].fd,buf,ret);
			}
		}
	}
	
	
	return 0;
}
