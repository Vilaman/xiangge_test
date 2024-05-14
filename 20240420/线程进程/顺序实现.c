/***********************************************************************************
# File Name:    顺序实现.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月08日 星期三 12时35分00秒
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

sem_t sems[NUMS];
void *demo(void *args);


int main() {
  for (int i = 0; i < NUMS; i++) {

    if (i == 0) {

      sem_init(&sems[i], 0, 1);
    } else {

      sem_init(&sems[i], 0, 0);
    }
  }
  pthread_t pids[NUMS];
  int index[NUMS];
  for (int i = 0; i < NUMS; i++) {

    index[i] = i + 1;
    if (pthread_create(&pids[i], NULL, demo, &index[i]) != 0) {

      perror("pthread_create()");
      exit(1);
    }
  }
  for (int i = 0; i < NUMS; i++) {

    pthread_join(pids[i], NULL);
  }
  for (int i = 0; i < NUMS; i++) {

    sem_destroy(&sems[i]);
  }

  return 0;
}


void *demo(void *args){

	int *indexs=(int *)args;
	sem_wait(&sems[*indexs-1]);
	//sleep(1);
	printf("线程%d正在执行\n",*indexs);
	if(*indexs<NUMS){
	sem_post(&sems[*indexs]);
	}

	pthread_exit(NULL);
}
