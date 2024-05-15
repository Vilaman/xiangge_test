/***********************************************************************************
# File Name:    server_main.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月14日 星期二 15时28分25秒
 **********************************************************************************/

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <semaphore.h>
#include <sqlite3.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include "server_error.h"
#include "server_function"



int main(){
	//创建服务器套接字
	int server_fd;
	if((server_fd=socket(AF_INET,SOCK_STREAM,0))<0){
		perror("server_socket");
		exit(1);
	}
	//绑定服务器信息
	struct sockaddr_in server;
	server.sin_family=AF_INET;
	server.sin_port=htons(PORT);
	server.sin_addr.s_addr=INADDR_ANY;

	//绑定端口
	if(bind(server_fd,(struct sockaddr_in *)&server,sizeof(server))<0){
		perror("bind");
		exit(2);
	}
	//监听
	if(listen(server_fd,15)<0){

		perror("listen()");
		exit(3);
	}
	//定义客户端相关信息
	struct sockaddr_in client;
	int client_fd;
	socklen_t len=sizeof(client);
	puts("等待连接");







    return 0;
}
