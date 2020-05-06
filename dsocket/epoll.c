#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <ctype.h>

#define PORT 6666

void sys_err(char *str)
{
	perror(str);
	exit(-1);
}
int main(int argc,char *argv[])
{
	int lfd,ret,retNUm;
	struct sockaddr_in sAddr,cAddr;
	socklen_t cAddrLen;
	struct  epoll_event event[BUFSIZ],tEvent;
	char bufIp[BUFSIZ],buf[BUFSIZ];
	
	sAddr.sin_family = AF_INET;
	sAddr.sin_port = htons(PORT);
	sAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	lfd = socket(AF_INET,SOCK_STREAM,0);
	if(lfd == -1)
		sys_err("socket failure!\n");
	
	int opt = 1;
	setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	
	ret = bind(lfd,(struct sockaddr *)&sAddr,sizeof(sAddr));
	if(-1 == ret )
		sys_err("bind failure!\n");
	
	ret = listen(lfd,128);
	if(-1 == ret)
		sys_err("listen failure!\n");
	
	int epfd;
	epfd = epoll_create(32);
	if(-1 == epfd)
		sys_err("epoll_create failure!\n");
	
	tEvent.events = EPOLLIN;
	tEvent.data.fd = lfd;
	ret = epoll_ctl(epfd,EPOLL_CTL_ADD,lfd,&tEvent);
	if(-1 == ret)
		sys_err("epoll_ctl failure!\n");
	
	while(1)
	{		
		retNUm = epoll_wait(epfd,event,BUFSIZ,0);
		if(-1 == retNUm)
			sys_err("epoll_wait failure!\n");
		
		
		for(int i = 0;i<retNUm;i++)
		{
			if(lfd == event[i].data.fd)
			{
				ret = accept(lfd,(struct sockaddr*)&cAddr,&cAddrLen);
				if(-1 == ret)
					sys_err("accept failure!\n");
				printf("[connected] IP = %s,fd = %d\n",inet_ntop(AF_INET,(void*)&cAddr.sin_addr.s_addr,bufIp,BUFSIZ),ret);
				
				tEvent.events = EPOLLIN;
				tEvent.data.fd = ret;
				
				ret = epoll_ctl(epfd,EPOLL_CTL_ADD,ret,&tEvent);
				if(-1 == ret)
					sys_err("epoll_ctl failure!\n");				
				
			}
			else{
				if((ret = read(event[i].data.fd,buf,BUFSIZ)) == 0)
				{					
					ret = epoll_ctl(epfd,EPOLL_CTL_DEL,event[i].data.fd,NULL);
					if(-1 == ret )
						sys_err("epoll_ctl");
					
					close(event[i].data.fd);
				}
				else
				{
					for(int j = 0;j<ret;j++)
						buf[j] = toupper(buf[j]);
					write(event[i].data.fd,buf,ret);
				}
			}
		}
	}
	return 0;
}