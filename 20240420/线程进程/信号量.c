/***********************************************************************************
# File Name:    信号量.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月08日 星期三 11时02分30秒
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
#define NUMS 5

sem_t sem;
int num=5;
void *demo(void *args);

int main(){
	sem_init(&sem,0,1);

	pthread_t pids[NUMS];
	int index[NUMS];
	for(int i=0;i<NUMS;i++){

		index[i]=i+1;
		if(pthread_create(&pids[i],NULL,demo,&index[i])!=0){
		
			perror("pthread_create()");
			exit(1);
		}
	}
	for(int i=0;i<NUMS;i++){

		pthread_join(pids[i],NULL);
	}
	sem_destroy(&sem);
	printf("全局变量%d\n",num);

    return 0;
}

void *demo(void *args){

	sem_wait(&sem);
	printf("线程%d执行\n",*(int *)args);
	if(num<=5){
		sleep(1);
		num++;
	}else{

		num*=2;
	}
	sem_post(&sem);
	pthread_exit(NULL);
}




