/***********************************************************************************
# File Name:    udpserver.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月06日 星期一 14时27分48秒
 **********************************************************************************/

#include <stdio.h>
#include<string.h> 
#include<stdbool.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>
int main(){
	
	int server_fd=socket(AF_INET,SOCK_DGRAM,0);
	if(server_fd<0){

		perror("socket()");
		exit(1);
	}
	struct sockaddr_in server;
	server.sin_family=AF_INET;
	server.sin_port=htons(8088);
	server.sin_addr.s_addr=INADDR_ANY;

	if(bind(server_fd,(struct sockaddr *)&server,sizeof(server))<0){

		perror("bind()");
		exit(1);
	}
	puts("等待接收数据");

	struct sockaddr_in client;
	socklen_t len=sizeof(client);
	int readcount;
	char data[51]="";
	do{
	if((readcount=recvfrom(server_fd,data,sizeof(data)-1,0,(struct sockaddr *)&client,&len))<0){
		perror("recvfrom()");
		exit(1);
	}
	data[readcount]='\0';
	printf("接收到的客户端数据是:%s\n",data);
	
	char returndata[50]="";
	puts("发送的数据");
	scanf(" %s",returndata);
	int writecount;
	if((writecount=sendto(server_fd,returndata,sizeof(returndata),0,(struct sockaddr *)&client,sizeof(client)))<0){

		perror("sendto()");
		exit(1);
	}
	printf("服务器发送的数据%s\n",returndata);
	}while(true);

	close(server_fd);

    return 0;
}
