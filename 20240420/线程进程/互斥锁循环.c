/***********************************************************************************
# File Name:    互斥锁循环.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月08日 星期三 15时19分10秒
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

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

int num=2;
void *demo(void *args);

int main(){
	pthread_t pids[4];
	int index[4];

	for(int i=0;i<4;i++){

		index[i]=i;
		if(pthread_create(&pids[i],NULL,demo,&index[i])!=0){

			perror("pthread_create()");
			exit(1);
		}

	}
	for(int i=0;i<4;i++){

		pthread_join(pids[i],NULL);
	}


    return 0;
}


void *demo(void *args){
//	pthread_mutex_lock(&mutex);
	int *i=(int *)args;
	printf("线程%d正在运行\n",*i+1);
	for(int j=0;j<20;j++){
		usleep(100);
		pthread_mutex_lock(&mutex);
		printf("当前执行的线程是%d:num的值为%d\n",*i+1,num);
		num++;
		pthread_mutex_unlock(&mutex);

	}
	printf("num的值为%d\n",num);
//	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}


