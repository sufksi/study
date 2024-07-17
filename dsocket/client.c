#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>


#define PORT 9999
int main()
{
	struct sockaddr_in st_caddr;
	int ret;
	char str[BUFSIZ];
	st_caddr.sin_family = AF_INET;
	st_caddr.sin_port = htons(PORT);
	inet_pton(AF_INET,"127.0.0.1",&st_caddr.sin_addr.s_addr);
	//st_caddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	int cfd;
	cfd = socket(AF_INET,SOCK_STREAM,0);
	connect(cfd,(struct sockaddr *)&st_caddr,sizeof(st_caddr));
	for(int i = 0;i<10;i++)
	{
				
		write(cfd,"hello\n",6);
		ret = read(cfd,str,sizeof(str));
		
		write(STDOUT_FILENO,str,ret);
		sleep(1);
	}
	
	close(cfd);
	return 0;
}