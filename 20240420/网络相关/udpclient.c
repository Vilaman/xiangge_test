/***********************************************************************************
# File Name:    udpclient.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月06日 星期一 13时57分50秒
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

	int client_fd=socket(AF_INET,SOCK_DGRAM,0);
	if(client_fd<0){
		perror("socket()");
		exit(1);
	}
	struct sockaddr_in server;
	server.sin_family=AF_INET;
	server.sin_port=htons(8088);
	server.sin_addr.s_addr=inet_addr("127.0.0.1");
	int writecount;
	char buf[50]="";
do{
	puts("请输入要发送的数据");
	scanf(" %s",buf);
	
	if((writecount=sendto(client_fd,buf,sizeof(buf),0,(struct sockaddr *)&server,sizeof(server)))<0){

		perror("sendto");
		exit(2);
	}
	printf("发送的是%s\n",buf);
	int readcount;
	char recivedata[51]="";
	socklen_t len=sizeof(server);
	if((readcount=recvfrom(client_fd,recivedata,sizeof(recivedata)-1,0,(struct sockaddr *)&server,&len))>0){
		recivedata[readcount]='\0';
		printf("接收到服务器的数据是%s\n",recivedata);
	}
	}while(true);
	close(client_fd);

    return 0;
}
