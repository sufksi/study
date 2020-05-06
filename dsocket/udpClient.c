#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>

#define PORT 8888
void sys_err(char *str)
{
	perror(str);
	exit(-1);
}

int main()
{
	int cfd,ret;
	char buf[BUFSIZ];
	struct sockaddr_in sAddr;
	socklen_t cAddrLen;
	cfd = socket(AF_INET,SOCK_DGRAM,0);
	if(-1 == cfd )
		sys_err("socket failure!\n");
	
	sAddr.sin_family = AF_INET;
	sAddr.sin_port = htons(PORT);
	sAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//inet_pton(AF_INET,"127.0.0.1",&sAddr.sin_addr.s_addr);
	//sAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	//hile(fgets(buf,BUFSIZ,stdin)!= NULL)
	while(1)
	{
		printf("cli:>");
		scanf("%s",buf);
		
		ret = sendto(cfd,buf,strlen(buf),0,(struct sockaddr*)&sAddr,sizeof(sAddr));
		
		ret = recvfrom(cfd,buf,BUFSIZ,0,NULL,0);
		if(-1 == ret )
		{
			sys_err("recvfrom failure!\n");
		}
		printf("cli:>%s\n",buf);
	}
	close(cfd);
	return 0;
}