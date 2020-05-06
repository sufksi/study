#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <stddef.h>
#include <sys/un.h>

#define SOCKNAME "srv.sock"

void sys_err(char *str)
{
	perror(str);
	exit(-1);
}

int main()
{
	int sfd,len,ret,cfd;
	struct sockaddr_un sAddr;
	socklen_t sAddrLen;
	char buf[BUFSIZ];
	
	sfd = socket(AF_UNIX,SOCK_STREAM,0);
	if(-1 == sfd)
		sys_err("socket failure!\n");
	
	sAddr.sun_family = AF_UNIX;
	strcpy(sAddr.sun_path,SOCKNAME);
	sAddrLen = offsetof(struct sockaddr_un,sun_path) + sizeof(SOCKNAME);
	
	unlink(SOCKNAME);
	ret = bind(sfd,(struct sockaddr *)&sAddr,sAddrLen);
	if(-1 == ret)
		sys_err("bind failure!\n");
	
	listen(sfd,128);
	
	while(1)
	{
		cfd = accept(sfd,(struct sockaddr *)&sAddr,&sAddrLen);
		if(-1 == cfd)
			sys_err("accept failure!\n");
		
		len = sAddrLen - offsetof(struct sockaddr_un,sun_path);
		sAddr.sun_path[len] = '\0';
		
		printf("cliName:%s\n",sAddr.sun_path);
		
		while((ret = read(cfd,buf,BUFSIZ)) > 0)
		{
			//ret = strlen(buf);
			printf("read:%s,num = %d\n",buf,ret);
			for(int i = 0; i < ret;i++)
			{
				buf[i] = toupper(buf[i]);
			}
			write(cfd,buf,ret);
		}		
		
	}
	return 0;
}