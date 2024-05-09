/***********************************************************************************
# File Name:    互斥锁.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月08日 星期三 14时16分53秒
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
#define NUMS 10


//互斥锁静态初始化不需要进行销毁
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
void *demo(void *args);

int num=5;

int main(){

	pthread_t pids[NUMS];
	int indexs[NUMS];
	for(int i=0;i<NUMS;i++){
		indexs[i]=i;
		if(pthread_create(&pids[i],NULL,demo,&indexs[i])!=0){
			
			perror("pthread_create()");
			exit(1);
		}
	}

	for(int i=0;i<NUMS;i++){

		pthread_join(pids[i],NULL);
	}

	puts("main执行结束");

    return 0;
}


void *demo(void *args){
	int *index=(int *)args;
	//获取锁
	pthread_mutex_lock(&mutex);
	printf("线程%d正在执行\n",*index+1);
	if(num<10){

		sleep(1);
		num*=2;
	}else if(num<15){
		sleep(1);
		num*=3;
	}
	else{
		sleep(1);
		num++;
	}
	printf("num的值为:%d\n",num);

	//解锁
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}
