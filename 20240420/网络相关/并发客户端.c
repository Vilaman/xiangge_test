/***********************************************************************************
# File Name:    并发客户端.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月07日 星期二 17时16分15秒
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
#include<pthread.h>
#define PORT 8088




int main(){

	int client_fd=socket(AF_INET,SOCK_STREAM,0);
	if(client_fd<0){

		perror("socket()");
		exit(1);
	}
	struct sockaddr_in server;
	server.sin_family=AF_INET;
	server.sin_port=htons(PORT);
	server.sin_addr.s_addr=inet_addr("127.0.0.1");
	
	if((connect(client_fd,(struct sockaddr*)&server,sizeof(server)))<0){

		perror("connect()");
		exit(1);
	}
	puts("链接成功");

	char input[100];
	int writecount;
	while(true){

		puts("请输入发送的数据");
		scanf(" %s",input);
		writecount=write(client_fd,input,sizeof(input));
		if(writecount<0){

			perror("read()");
			exit(1);
		}else if(writecount==0){

			puts("服务器已关闭");
			exit(1);
		}else {

			puts("发送成功");

		}

	}
	int readcount;
	char data[101];
	readcount=read(client_fd,data,sizeof(data)-1);
	if(readcount>0){

		data[readcount]='\0';
		puts(data);
	}else{

		puts("服务器下线");
	}


	close(client_fd);

    return 0;
}
