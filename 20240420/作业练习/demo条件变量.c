/***********************************************************************************
# File Name:    demo条件变量.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月09日 星期四 12时02分44秒
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

pthread_cond_t cond=PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

bool isPrintA=true;

void *demoA(void *args);
void *demoB(void *args);


int main(){
	pthread_t pid1,pid2;
	if(pthread_create(&pid1,NULL,demoA,NULL)!=0){

		perror("pthread_create()");
		exit(1);
	}

	if(pthread_create(&pid2,NULL,demoB,NULL)!=0){

		perror("pthread_create()");
		exit(1);
	}


	pthread_join(pid1,NULL);
	pthread_join(pid2,NULL);

	pthread_cond_destroy(&cond);

    return 0;
}


void *demoA(void *args){
	pthread_mutex_lock(&mutex);
	printf("执行A的一部分打印\n");
	pthread_cond_wait(&cond,&mutex);
	printf("执行A的剩余部分打印\n");
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}


void *demoB(void *args){
//	usleep(100);//避免B先抢到时间片
	pthread_mutex_lock(&mutex);
	printf("打印B全部\n");
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}

