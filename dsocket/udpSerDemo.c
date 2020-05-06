//ser.c
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
 
#include<sys/socket.h>
#include<netinet/in.h>
#include <arpa/inet.h>
 
int main()
{
    int sockSer = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockSer == -1)
    {
        perror("socket");
        exit(1);
    }
 
    struct sockaddr_in addrSer;
    addrSer.sin_family = AF_INET;
    addrSer.sin_port = htons(8080);//端口号
    addrSer.sin_addr.s_addr = inet_addr("192.168.31.6");//服务器地址
 
    socklen_t addrlen = sizeof(struct sockaddr);
    int ret = bind(sockSer, (struct sockaddr*)&addrSer, addrlen);
    if(ret == -1)
    {
        perror("bind.");
        exit(1);
    }
 
    struct sockaddr_in addrCli;
 
    char sendbuf[256];
    char recvbuf[256];
    while(1)
    {
        recvfrom(sockSer, recvbuf, 256, 0, (struct sockaddr*)&addrCli, &addrlen);
        printf("Cli:>%s\n", recvbuf);
        printf("Ser:>");
        scanf("%s",sendbuf);
        sendto(sockSer, sendbuf, strlen(sendbuf)+1, 0, (struct sockaddr*)&addrCli, addrlen);
    }
 
    close(sockSer);
    return 0;
}
 
 
 