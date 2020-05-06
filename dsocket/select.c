//实现select多路IO转接
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <ctype.h>

void sys_err(const char *str)
{
	perror(str);
	exit(1);
}


#define PORT 9999
int main(int argc,char *argv[])
{
	int lfd,cfd;
	int ret;
	int maxfd;
	fd_set lisfd,lstfd_bak;
	char buf[BUFSIZ];
	char bufIp[BUFSIZ];
	struct sockaddr_in sAddr,cAddr;
	sAddr.sin_family = AF_INET;
	sAddr.sin_port = htons(PORT);
	sAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	socklen_t addrlen;
	lfd = socket(AF_INET,SOCK_STREAM,0);
	if(lfd == -1)
		sys_err("creat socket failure!\n");
	
	
	ret = bind(lfd,(struct sockaddr *)&sAddr,sizeof(sAddr));
	if(ret != 0){
		sys_err("bind error!\n");
	}
	
	ret = listen(lfd,128);
	if(ret != 0){
		sys_err("listen error!\n");
	}
	FD_ZERO(&lstfd_bak);
	FD_SET(lfd,&lstfd_bak);
	maxfd = lfd;
	addrlen = sizeof(cAddr);
	while(1)
	{
		lisfd = lstfd_bak;
		ret = select(maxfd + 1,&lisfd,NULL,NULL,NULL);
		if(-1 == ret)
		{
			sys_err("select failure!\n");
		}
		else if(ret > 0)
		{			
			if(FD_ISSET(lfd,&lisfd))
			{
				cfd = accept(lfd,(struct sockaddr*)&cAddr,&addrlen);
				if(cfd == -1)
				{
					sys_err("accept failure!\n");
				}
				printf("[connected] IP = %s,fd = %d\n",inet_ntop(AF_INET,(void*)&cAddr.sin_addr.s_addr,bufIp,BUFSIZ),cfd);
				FD_SET(cfd,&lstfd_bak);
					
				if(cfd > maxfd)
					maxfd = cfd;
					
				if(ret == 1)
					continue;				
				
			}
			for(int i = lfd;i<=maxfd;i++)
			{
				//printf(" i = %d,maxfd = %d\n",i,maxfd);
				if(FD_ISSET(i,&lisfd))
				{
					if((ret = read(i,buf,sizeof(buf))) == 0)
					{
						FD_CLR(i,&lstfd_bak);
						close(i);						
						printf("[close] fd = %d\n",i);
						continue;
					}					
					
					for(int j = 0;j<ret;j++)
					{
						buf[j] = toupper(buf[j]);
					}	
					write(i,buf,ret);
					write(STDOUT_FILENO,buf,ret);
				}
			}		
			
		}
	}
	close(lfd);
	
	return 0;
}
