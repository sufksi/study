#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h> 
#include <unistd.h>

#define PORT 8888
void sys_err(char *str)
{
	perror(str);
	exit(-1);
}

int main()
{
	int sfd,ret;
	struct sockaddr_in sAddr,cAddr;
	socklen_t cAddrLen = sizeof(cAddr);
	char buf[BUFSIZ];
	sfd = socket(AF_INET,SOCK_DGRAM,0);
	if(-1 == sfd)
		sys_err("socket failure!\n");
	
	sAddr.sin_family = AF_INET;
	sAddr.sin_port = htons(PORT);	
	sAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//sAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	ret = bind(sfd,(struct sockaddr*)&sAddr,sizeof(sAddr));
	if(-1 == ret)
		printf("bind failure!\n");
	char bufIp[BUFSIZ];
	while(1)
	{
		ret = recvfrom(sfd,buf,BUFSIZ,0,(struct sockaddr *)&cAddr,&cAddrLen);
		if(-1 == ret )
		{
			sys_err("recvfrom failure!\n");
		}
		else if(0 == ret)
		{
			
			printf("[close]\tip:%s\tport:%d\n",inet_ntop(AF_INET,(void*)&cAddr.sin_addr.s_addr,bufIp,BUFSIZ),cAddr.sin_port);
			continue;
		}
		else
		{
			printf("[recvform]\tip:%s\tport:%d\n",inet_ntop(AF_INET,(void*)&cAddr.sin_addr.s_addr,bufIp,BUFSIZ),cAddr.sin_port);
			for(int i=0;i<ret;i++)
				buf[i] = toupper(buf[i]);
			
			sendto(sfd,buf,ret,0,(struct sockaddr*)&cAddr,sizeof(cAddr));
		}
	}
	close(sfd);
	return 0;
}