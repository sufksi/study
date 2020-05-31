#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <ctype.h>
#include <string.h>

#define DEBUG
#ifdef DEBUG
#define PRI(fmt, ...)\
	do{\
		fprintf(stdout, "[f:%s l:%d]", __FUNCTION__, __LINE__ );\
		fprintf(stdout, fmt, ##__VA_ARGS__);\
		}while(0)
#else
#define PRI(fmt, ...)
#endif

#define PORT 6666

#define ALL_START '{'
#define ALL_END '}'
#define MOULE_START '['
#define MOULE_END ']'
#define MODE_START '('
#define MODE_END ')'
#define VALUE_SEP ','
#define END			';'

 typedef enum  SCOKET_CMD{
        READ_FORM_SEVER_ALL = 0,
        READ_FORM_SEVER_ONE_MODULE,
        READ_FORM_SEVER_ONE_VALUE,
        READ_FORM_SEVER_RUN_MODE,

        WRITE_TO_SEVER_ALL,
        WRITE_TO_SEVER_ONE_MODULE,
        WRITE_TO_SEVER_ONE_VALUE,
        WRITE_TO_SEVER_RUN_MODE,

    }socket_cmd;

void sys_err(char *str)
{
	perror(str);
	exit(-1);
}
int parseMode(int mode[],const char *buf,int bufSize)
{
	int ret = 0;
	char bufData[10] = {0};
	int bufDataIndex = 0;
	int index = 1,modIndex = 0;
	int value;
	if(buf[0] == MODE_START && buf[bufSize - 1] == MODE_END)
	{
		bufDataIndex = 0;
		//while(buf[index] != '\0')
		for(index = 1;index<bufSize;index++)
		{						
			if(buf[index] != VALUE_SEP&& buf[index] != MODE_END)
			{
				bufData[bufDataIndex++] = buf[index];				
			}
			else
			{
				bufData[bufDataIndex] = '\0';
				sscanf(bufData,"%d",&mode[modIndex++]);
				bufDataIndex = 0;
			}
			//index++;					
		}
	}
	else 
	{
		for(int i = 0;i<bufSize;i++)
		{
			printf("%c",buf[i]);
		}
		//printf("%c- %c\n",buf[0],buf[bufSize - 1]);
		PRI("mode data error!\n");
		return -1;
	}
	return ret;
}
int parseModule(int *enStatus,int moduleValue[][16], char *buf,int bufSize)
{
	int ret = 0;
	int index = 0;
	int modeSize = 0,modeIndex = 0;
	char *modeBegin;
	char enStatusBuf[10] = {0};
	int enStarIndex = 0;
	if(buf[0] == MOULE_START && buf[bufSize - 1] == MOULE_END)
	{
		while(buf[index++] != MODE_START);
		
		while(buf[index] != MODE_END)
			enStatusBuf[enStarIndex++] = buf[index++];
		enStatusBuf[enStarIndex] = '\0';
		sscanf(enStatusBuf,"%d,%d,%d",&enStatus[0],&enStatus[1],&enStatus[2]);		
		
		for(index++;index <bufSize;index++)
		{
			if(buf[index] == MODE_START)
			{
				modeSize = 1;
				modeBegin = &buf[index];				
			}
			else if(buf[index] == MODE_END)
			{
				parseMode(moduleValue[modeIndex++],modeBegin,modeSize + 1);
			}
			else
			{
				modeSize++;
			}
		}
	}
	else 
	{
		PRI("module data error!\n");
		return -1;
	}
	
	return ret;
}

void printMoudele(int *enStatus,int moduleValue[][16])
{
	printf("-------------------mode start -----------------\n");
	printf("auto = %d,sn_auto = %d,ex_iso = %d\n",enStatus[0],enStatus[1],enStatus[2]);
	for(int i = 0;i<3;i++)
	{
		for(int j = 0;j<16;j++)
			printf("%d\t",moduleValue[i][j]);
		printf("\n");
	}
}
#if 0
int comPacket(socket_cmd cmd,int mod,int row,int col,int *arg)
{
    char str[BUFSIZ] = {'\0'};
	int index=0;
    str[index++] = '#';
	str[index++] = '#';
	str[index++] = '#';
	str[index++] = (char)(cmd + 0x30);
    str[index++] = '#';

    switch (cmd) {
        case READ_FORM_SEVER_ALL:/*全读*/			
            str.append('#');
            str.append(";");
            break;
        case READ_FORM_SEVER_ONE_MODULE:/*读一个模块*/
            str.append('#');
            str.append(QString::number(mod));
            str.append('#');
            str.append(';');
            break;
        case READ_FORM_SEVER_ONE_VALUE:/*读一个模块中的一个值*/ 
			sprintf(str + strlen(str),"%d",mod);
			sprintf(str + strlen(str),"%c",VALUE_SEP);
			sprintf(str + strlen(str),"%d",row);
			sprintf(str + strlen(str),"%c",VALUE_SEP);
			sprintf(str + strlen(str),"%d",col);
			sprintf(str + strlen(str),"%c",VALUE_SEP);
			sprintf(str + strlen(str),"%d",*arg);
			sprintf(str + strlen(str),"%c",';');
			
            break;
        case READ_FORM_SEVER_RUN_MODE:

            break;
        case WRITE_TO_SEVER_ALL:/*全写*/
            writeAll(str);
            str.append(';');
            break;
        case WRITE_TO_SEVER_ONE_MODULE:/*写一个模块*/
            str.append(QString::number(mod));
            str.append(VALUE_SEP);
            writeModule(mod,str);
            str.append(';');
            break;
        case WRITE_TO_SEVER_ONE_VALUE:/*写一个模块中的一个值*/
            writeOneValue(mod,row,col,str);
            str.append(';');
            break;
        case WRITE_TO_SEVER_RUN_MODE:
            writeRunMode(mod,str);
            break;
        default:
            qDebug()<<"invalid cmd";
            str.clear();
         break;

    }
    //str.append('\0');
    socketWriteToSever(str);
    return str;
}
#endif
int dealRead(char *buf,int bufsize)
{
	int nStr = strlen(buf);
	char bufRead[1024];	
	int index = 0,indexBufRead;
	int module,mode,pos,value;
	socket_cmd cmd;
	int moduleValue[3][16] = {0};
	int enStatus[3] = {0};
	int tmp = 0;
	char *moduleBegin;
	
	if('#' == buf[0]&&'#' == buf[1] &&'#'== buf[2])
	{
		index = 3;
		indexBufRead = 0;
		while(buf[index] != '#')
		{
			bufRead[indexBufRead++] = buf[index++];
		}
		index++;
		bufRead[indexBufRead] = '\0';
		cmd = atoi(bufRead);
		switch(cmd)
		{			
			case READ_FORM_SEVER_ALL:/*全读*/
				
				break;
			case READ_FORM_SEVER_ONE_MODULE:/*读一个模块*/
				
				break;
			case READ_FORM_SEVER_ONE_VALUE:/*读一个模块中的一个值*/
				
				break;
			case WRITE_TO_SEVER_ALL:/*全写*/
				indexBufRead = 0;
				tmp = 0;
				
				while(buf[index] != ALL_END)
				{
					if(buf[index] == MOULE_START)
					{
						moduleBegin = &buf[index];
						indexBufRead = 0;
					}
					else if(buf[index] == MOULE_END)
					{
						parseModule(enStatus,moduleValue,moduleBegin,indexBufRead + 1);
						PRI("module = %d\n",tmp);
						printMoudele(enStatus,moduleValue);
					}				
						
					indexBufRead++;
					index++;
				}			
				
				break;
			case WRITE_TO_SEVER_ONE_MODULE:/*写一个模块*/	
				/*###5#1,[(2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0)(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)];*/
				indexBufRead = 0;
				while(buf[index] != VALUE_SEP)
				{
					bufRead[indexBufRead++] = buf[index++];
				}
				bufRead[indexBufRead] = '\0';
				sscanf(bufRead,"%d",&module);
				PRI("module = %d\n",module);
				
				index++;				
				
				while(buf[index] != MOULE_END)
				{
					if(buf[index] == MOULE_START)
					{
						moduleBegin = &buf[index];
						indexBufRead = 0;
					}
						
					indexBufRead++;
					index++;
				}
				
				parseModule(enStatus,moduleValue,moduleBegin,indexBufRead + 1);
				printMoudele(enStatus,moduleValue);
				break;
			case WRITE_TO_SEVER_ONE_VALUE:/*写一个模块中的一个值*/
				/*###6#2,0,5,0;*/
				indexBufRead = 0;
				while(buf[index] != END)
				{
					bufRead[indexBufRead++] = buf[index++];
				}
				bufRead[indexBufRead] = '\0';
				sscanf(bufRead,"%d,%d,%d,%d",&module,&mode,&pos,&value);
				printf("ONE_VALUE = %d,%d,%d,%d",module,mode,pos,value);
				break;
			default:
				break;
		}

    }
		
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
					buf[ret] = '\0';
					dealRead(buf,sizeof(buf));
					//printf("[server recv data](%s)\n",buf);
					/*for(int j = 0;j<ret;j++)
						buf[j] = toupper(buf[j]);
					write(event[i].data.fd,buf,ret);*/
				}
			}
		}
	}
	return 0;
}