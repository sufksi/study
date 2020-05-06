#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>


#define PORT 9999
#define BUFSIZE 1024
void sys_err(char *s)
{
	perror(s);
	exit(1);
}	

void deal()
{
	pid_t pid = waitpid(0,NULL,WNOHANG);
	printf("pid %d exit\n",pid);
	return;
}
int main()
{
	struct sockaddr_in st_saddr,st_caddr;
	signal(SIGCHLD,deal);
	
	int ret,i;
	int lfd,cfd;
	char buf[BUFSIZE];
	socklen_t caddr_len;
	memset(buf,0,BUFSIZE);

	fflush(stdout);
	st_saddr.sin_family = AF_INET;
	st_saddr.sin_port = htons(PORT);
	st_saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	lfd = socket(AF_INET,SOCK_STREAM,0);
	
	ret = bind(lfd,(struct sockaddr *)&st_saddr,sizeof(st_saddr));
	if(ret != 0)
		sys_err("bind failure\n");
	
	caddr_len = sizeof(st_caddr);
	listen(lfd,128);	
	
	while(1)
	{
		pid_t pid;
		
		cfd = accept(lfd,(struct sockaddr *)&st_caddr,&caddr_len);
		if(ret == -1)
			sys_err("accept failure\n");
		if((pid = fork()) <0)
		{
			sys_err("fork failure\n");
		}
		else if(0 == pid)
		{			
			close(lfd);
			while((ret = read(cfd,buf,sizeof(buf))) != 0)
			{
				for(i = 0; i < ret; i++)
				{
					buf[i] = toupper(buf[i]);
				}
				write(cfd,buf,strlen(buf) - 1);
			}		
			
			close(cfd);
			return 0;
		}
		else{
			close(cfd);
		}
	}
	
	return 0;
}
