/***********************************************************************************
# File Name:    并发服务器.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月07日 星期二 17时16分32秒
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
#include<semaphore.h>
#include<sys/wait.h>


#define PORT 8088

typedef struct Client {
  struct sockaddr_in info;
  int fd;
} Client;

sem_t sem;

void *hanleclient(void *args);
void writetoLog(char *info);

int main(){

	pthread_t pid1;
	int clientfd;
	int server_fd=socket(AF_INET,SOCK_STREAM,0);
	if(server_fd<0){

		perror("socket()");
		exit(1);
	}
	struct sockaddr_in server;
	server.sin_family=AF_INET;
	server.sin_port=htons(PORT);
	server.sin_addr.s_addr=INADDR_ANY;

	if((bind(server_fd,(struct sockaddr *)&server,sizeof(server)))<0){

		perror("bind()");
		exit(1);
	}
	if(listen(server_fd,10)<0){
		
		perror("listen()");
		exit(1);
	}
	puts("等待客户端连接");

	struct sockaddr_in client;
	socklen_t len=sizeof(client);
	while((clientfd=accept(server_fd,(struct sockaddr *)&client,&len))>0){
		sem_init(&sem,0,1);
		puts("已接收客户端链接");
		Client *c=(Client *)malloc(sizeof(Client));
		char *uip=inet_ntoa(client.sin_addr);
		int uport=ntohs(client.sin_port);
		c->info=client;
		c->fd=clientfd;

		printf("接收到来自:%s:%d的连接\n",uip,uport);
		puts("等待下一位连接");
	//	if(pthread_create(&pid,NULL,hanleclient,c)!=0){
//
//			puts("创建连接失败");
//			exit(1);
//		}	
		int pid=fork();
		if(pid>0){
			close(clientfd);
		}else if(pid==0){
			hanleclient(c);
			exit(0);
		}
		sem_destroy(&sem);

	}
	int writecount;
	char input[100]="erwrewtwtffffds";

	writecount=write(clientfd,input,sizeof(input));
	if(writecount>0){
		puts("发送成功");
		puts(input);
	}

	

close(server_fd);
    return 0;
}




void *hanleclient(void *args) {
  int clientfd = ((Client *)args)->fd;
  pthread_t tid = pthread_self();
  pthread_detach(tid);
  Client *c=(Client*)(args);
  char *ip=inet_ntoa(c->info.sin_addr);
  int port=htons(c->info.sin_port);
  char big[60];
  sprintf(big, "%s:%d", ip, port);
  writetoLog(big);
  int readcount;
  char data[101];
  printf("客户端%d连接\n", clientfd);
  while ((readcount = read(clientfd, data, sizeof(data) - 1)) > 0) {

    data[readcount] = '\0';
    printf("服务器接收数据:%s\n", data);
  }
  puts("客户端下线");
  free(args);
  args = NULL;
  pthread_exit(NULL);
}



void writetoLog(char *info){
	sem_wait(&sem);
	FILE *pf=fopen("connectlog.txt","a");
	if(pf==NULL){
		perror("fopen");
		exit(2);
	}
	char status[]="login";
	time_t t=time(NULL);
	struct tm *now=localtime(&t);
	char timestr[40];
	strftime(timestr,sizeof(timestr)-1,"%y年%m月%d日 %H:%M:%S",now);
	timestr[strlen(timestr)]='\0';
	fprintf(pf,"%s %s %s\n",info,timestr,status);
	puts("写入日志成功");
	fclose(pf);
	pf=NULL;
	sem_post(&sem);
}
