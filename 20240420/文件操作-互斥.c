/***********************************************************************************
# File Name:    文件操作-互斥.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月08日 星期三 16时40分45秒
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

void *fileread(void *args);
void *filewrite(void *args);

char data[1024]="";
//pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
sem_t sems[2];
int main(){
	sem_init(&sems[0],0,1);
	sem_init(&sems[1],0,0);
	pthread_t pid1,pid2;
	if(pthread_create(&pid1,NULL,fileread,NULL)!=0){

		perror("pthread_create()");
		exit(1);
	}
//	pthread_join(pid1,NULL);
	if(pthread_create(&pid2,NULL,filewrite,data)!=0){

		perror("pthread_create()");
		exit(1);
	}
	pthread_join(pid1,NULL);
	pthread_join(pid2,NULL);
	sem_destroy(&sems[0]);
	sem_destroy(&sems[1]);

	puts("main 结束");
    return 0;
}

void *fileread(void *args){
	sem_wait(&sems[0]);//等待信号量
	FILE *fd=fopen("sour.txt","r");
	if(fd==NULL){

		perror("fopen()");
		exit(1);
	}
	char redata[50]="";
	while(fgets(redata,sizeof(redata),fd)!=NULL){
		puts(redata);	
		strcat(data,redata);
	
	}
	puts(data);
	fclose(fd);
	fd=NULL;
	sem_post(&sems[1]);//释放信号量
	pthread_exit(NULL);
}


void *filewrite(void *args){
	sem_wait(&sems[1]);
	char *data=(char *)args;
	FILE *pf=fopen("dest.txt","w");
	if(pf==NULL){

		perror("fopen()");
		exit(1);
	}
	puts(data);
	fputs(data,pf);	
	fclose(pf);
	pf=NULL;
	sem_post(&sems[1]);
	pthread_exit(NULL);

}
