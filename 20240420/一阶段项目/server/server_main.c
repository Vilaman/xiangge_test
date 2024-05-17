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
#include "server_function.h"
#include "pthread_pool.h"
#include "server_info.h"



int main(){
	//创建服务器套接字
	pthread_pool pool;
	pool_init(&pool,5,10);
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
	if(bind(server_fd,(struct sockaddr*)&server,sizeof(server))<0){
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
	while((client_fd =accept(server_fd,(struct sockaddr*)&client,&len))>0){
		//已连接
		//处理客户端连接
		//保证每个客户端连接保存套接字的是一个新的变量和空间，彼此线程互不影响
	    Client *c = malloc(sizeof(Client));
		c->client_fd = client_fd;
		c->info =client;

		pthread_pool_add_task(&pool,handleClient,c);
		//输出客户端已连接
		
		char *ip = inet_ntoa(client.sin_addr);
		int port = ntohs(client.sin_port);
		printf("已接受客户端:%s:%d的连接\n",ip,port);
		//等待下一位客户端连接
		puts("等待下一位客户端的连接");
	}
	//5. 关闭
	close(server_fd);
    return 0;
}

//传递的就是指向客户端套接字的指针
void *handleClient(void *args) {
  //分离
  //pthread_t pid = pthread_self();
  //pthread_detach(pid); //设置为分离线程
  Client *c = (Client *)args;
  printf("%d\n",c->client_fd);
  server_Mainop(c);
  //free(args); //释放保存客户端套接字的内存，因为是malloc分配的
}
