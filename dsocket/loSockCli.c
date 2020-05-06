#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/un.h>


#define SOCKNAME "srv.sock"
#define SOCKNAMECLI "cli.sock"
void sys_err(char *str)
{
	perror(str);
	exit(-1);
}

int main()
{
	int cfd,ret;
	char buf[BUFSIZ];
	struct sockaddr_un cAddr;
	socklen_t cAddrLen;
	
	cfd = socket(AF_UNIX,SOCK_STREAM,0);
	if(-1 == cfd)
		sys_err("socket failure!\n");
	
	cAddr.sun_family = AF_UNIX;
	strcpy(cAddr.sun_path,SOCKNAMECLI);
	
	cAddrLen = offsetof(struct sockaddr_un,sun_path) + sizeof(SOCKNAMECLI);
	
	unlink(SOCKNAMECLI);
	ret = bind(cfd,(struct sockaddr*)&cAddr,cAddrLen);
	if(-1 == ret)
			sys_err("bind failure!\n");
	
	bzero(&cAddr,sizeof(cAddr));
	cAddr.sun_family = AF_UNIX;
	strcpy(cAddr.sun_path,SOCKNAME);
	
	cAddrLen = offsetof(struct sockaddr_un,sun_path) + sizeof(SOCKNAME);
	ret = connect(cfd,(struct sockaddr*)&cAddr,cAddrLen);
	if(-1 == ret)
		sys_err("connect failure!\n");
		
	while(1)
	{
		printf("\ncli:>");
		scanf("%s",buf);
		buf[strlen(buf)] = '\0';
		ret = write(cfd,buf,strlen(buf) + 1);
		if(-1 == ret)
			sys_err("write failure!\n");		
		
		ret = read(cfd,buf,BUFSIZ);
		if(-1 == ret)
			sys_err("read failure!\n");
		buf[ret] = '\0';
		printf("cli:>%s",buf);	
		
	}
	return 0;
}