/***********************************************************************************
# File Name:    110-并发客户端.c
# Author:       dengiswen
# Mail:         dengiswen@gmail.com
# Created Time: 2024年05月07日 星期二 16时42分28秒
 **********************************************************************************/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
void * readMessage(void*);
int client_fd;
int main(){
	//1.
	client_fd = socket(AF_INET,SOCK_STREAM,0);
	//2.声明服务器信息
	struct sockaddr_in server;
	server.sin_family=AF_INET;
	server.sin_port=htons(8088);
	server.sin_addr.s_addr=inet_addr("127.0.0.1");
	//3连接
	if(connect(client_fd,(struct sockaddr*)&server,sizeof(server))<0){
		perror("连接失败");
		exit(0);
	}
	pthread_t pid;
	//定义新的线程来接受服务器的群发信息
	pthread_create(&pid,NULL,readMessage,&client_fd);
	//链接成功
	puts("成功连接到服务器");
	char data[100];
	int readCount;
	while(true){
		memset(data,0,sizeof(data));
		puts("请输入要发送的数据");
		scanf(" %99s",data);
		readCount = write(client_fd,data,sizeof(data));
		if(readCount==0){
			//服务器已关闭
			puts("服务器已关闭，终止连接");
			exit(0);
		}else if(readCount <0){
			perror("读取失败");
			exit(0);
		}else{
			//大于0，发送成功
			puts("发送数据成功");
		}
	}
	pthread_join(pid,NULL);
    return 0;
}
void * readMessage(void*args){
	int * fd = (int*)args;
	
	char data[1024]="";
	int readCount;
	while(1){
		puts("等待服务器群发");
		readCount = read(*fd,data,sizeof(data)-1);
		if(readCount >0){
			data[readCount] = '\0';
			puts("来自服务器群发的信息:");
			puts(data);
		}else if(readCount ==0){
			puts("服务器已关闭");
			pthread_exit(0);
		}else {
			puts("读取发生错误");
		}
	}
	return NULL;
}
