/***********************************************************************************
# File Name:    文件复制操作单行复制.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月09日 星期四 10时24分20秒
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

sem_t sems[2];
char data[100];//接收每行数据

void *fileread(void *args){
FILE *pf=fopen("1.txt","r");
if(pf==NULL){

	perror("fopen()");
	exit(1);
}
while(fgets(data,sizeof(data),pf)!=NULL){

	printf("read\n");
	sem_post(&sems[1]);//写的信号量+1
	sem_wait(&sems[0]);//等待信号量
	memset(data,0,sizeof(data));
}
sem_post(&sems[1]);
pthread_exit(NULL);

}

void *filewrite(void *args){

	FILE *pf=fopen("2.txt","a");
	if(pf==NULL){

		perror("fopen()");
		exit(1);
	}
	sem_wait(&sems[1]);
	while(strcmp(data,"")!=0){

		puts("write");
		fputs(data,pf);
		sem_post(&sems[0]);
		sem_wait(&sems[1]);
	}
	pthread_exit(NULL);
}


int main(){

    return 0;
}
