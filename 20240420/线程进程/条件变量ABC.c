/***********************************************************************************
# File Name:    条件变量ABC.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月09日 星期四 14时05分19秒
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

pthread_mutex_t mutex;
pthread_cond_t cond;

int num=1;

void *demoA(void *args);
void *demoB(void *args);
void *demoC(void *args);

int main(){
pthread_t pid1,pid2,pid3;
if(pthread_create(&pid1,NULL,demoA,NULL)!=0){
	perror("pthread_createA");
	exit(1);
}
if(pthread_create(&pid2,NULL,demoB,NULL)!=0){
	perror("pthread_createB");
	exit(1);
}
if(pthread_create(&pid3,NULL,demoC,NULL)!=0){
	perror("pthread_createC");
	exit(1);
}

pthread_join(pid1,NULL);
pthread_join(pid2,NULL);
pthread_join(pid3,NULL);

pthread_mutex_destroy(&mutex);
pthread_cond_destroy(&cond);

puts("main结束");
    return 0;
}

void *demoA(void *args) {
  for (int i = 0; i < 3; i++) {

    pthread_mutex_lock(&mutex);
    while (num != 3) {
      pthread_cond_wait(&cond, &mutex);
    }
    puts("A");
    num = 1;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
  }
  return NULL;
}

void *demoB(void *args) {
  for (int i = 0; i < 3; i++) {

    pthread_mutex_lock(&mutex);
    while (num != 1) {
      pthread_cond_wait(&cond, &mutex);
    }
    puts("B");
    num = 2;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
  }
  return NULL;
}


void *demoC(void *args) {
  for (int i = 0; i < 3; i++) {

    pthread_mutex_lock(&mutex);
    while (num != 2) {
      pthread_cond_wait(&cond, &mutex);
    }
    puts("C");
    num = 3;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
  }
  return NULL;
}
