/***********************************************************************************
# File Name:    操作全局-互斥.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月08日 星期三 17时29分26秒
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

int num=0;
void *demo1(void *args);
void *demo2(void *args);


int main(){
	pthread_t pid1,pid2;
	if(pthread_create(&pid1,NULL,demo1,NULL)!=0){

		perror("pthread_create()");
		exit(1);
	}
	pthread_join(pid1,NULL);
	
	if(pthread_create(&pid2,NULL,demo2,NULL)!=0){

		perror("pthread_create()");
		exit(1);
	}
	pthread_join(pid2,NULL);

    return 0;
}

void *demo1(void *args) {
usleep(200);
  for (int i = 0; i <= 5; i++) {
    printf("线程1执行num为:%d\n", num+1);
    num++;
  }
  pthread_exit(NULL);
}

void *demo2(void *args) {
  for (int i = 0; i < 4; i++) {
    printf("线程2执行num为:%d\n", num+1);
    num++;
  }
  pthread_exit(NULL);
}
