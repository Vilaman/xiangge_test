/***********************************************************************************
# File Name:    条件变量.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月08日 星期三 14时44分05秒
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

void *demoA(void *args);
void *demoB(void *args);
//pthread_cond_t cond=PTHREAD_COND_INITIALIZER;  //静态初始化条件锁

int pid=1;//表示让奇数执行的条件
int num=1;
int main(){
	//动态初始化条件锁和互斥锁
	pthread_cond_init(&cond,NULL);
	pthread_mutex_init(&mutex,NULL);

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

//销毁互斥锁和条件锁
	pthread_cond_destroy(&cond);
	pthread_mutex_destroy(&mutex);
    return 0;
}

//输出奇数函数
void *demoA(void *args) {
  sleep(1);
  while (num <= 10) {
    sleep(1);
    pthread_mutex_lock(&mutex); //上锁
    while (pid != 1 && num <= 10) {

      //当不满足是奇数信号时就变成等待执行状态(阻塞状态)同时释放互斥锁
      // pthread_cond_signal()随机唤醒一个等待线程，并让该条件持有互斥锁，pthread_cond_broadcast()唤醒所有等待线程但是最终只会有一个线程被唤醒，并持有互斥锁
      pthread_cond_wait(&cond, &mutex);
    }
    if (num <= 10) {

      num++;
      pid = 2; //让偶数可以执行
      puts("现在是奇数打印输出");
      pthread_cond_signal(&cond); //唤醒等待线程
    }

    pthread_mutex_unlock(&mutex);
  }
  pthread_exit(NULL);
}


//打印偶数输出
void *demoB(void *args) {
  while (num <= 10) {

    pthread_mutex_lock(&mutex);
    sleep(1);
    while (pid != 2 && num <= 10) {

      pthread_cond_wait(&cond, &mutex);
    }
    if (num <= 10) {

      num++;
      pid = 1; //把锁还给奇数
      puts("现在是偶数打印输出");
      pthread_cond_signal(&cond);
    }
    pthread_mutex_unlock(&mutex);
  }
  pthread_exit(NULL);
}
