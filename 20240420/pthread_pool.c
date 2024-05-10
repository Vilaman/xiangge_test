/***********************************************************************************
# File Name:    pthread_pool.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月10日 星期五 10时26分39秒
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
#include"pthread_pool.h"


//静态初始化
pthread_mutex_t mutex_pool=PTHREAD_MUTEX_INITIALIZER;

//初始化线程池
void pool_init(pthread_pool *pool,int thread_max,int queue_max){
	pool->threadCount=thread_max;
	pthread_t *tids=(pthread_t *)malloc(sizeof(pthread_t)*thread_max);
	pool->pids=tids;
	pool->idleCount=thread_max;
	
	sem_init(&pool->sem_tasks,0,0);
	sem_init(&pool->sem_workers,0,thread_max);

	//初始化队列
	task_queue *queue=(task_queue *)malloc(sizeof(task_queue));
	if(queue==NULL){

		perror("malloc");
		exit(1);
	}
	pool->queue=queue;
	queue->max_size=queue_max;
	queue->front=queue->rear=-1;
	queue->count=0;
	queue->tasks=(task *)malloc(sizeof(task)*queue_max);
	for(int i=0;i<thread_max;i++){
		pthread_create(&pool->pids[i],NULL,Exec,pool);
	}
	return;
}

//销毁线程池
void  pthread_pool_destroy(pthread_pool *pool){

	for(int i=0;i<pool->threadCount;i++){

		pthread_join(pool->pids[i],NULL);
	}
	free(pool->pids);
	pool->pids=NULL;
	sem_destroy(&pool->sem_tasks);
	sem_destroy(&pool->sem_workers);
	free(pool->queue->tasks);
	pool->queue->tasks==NULL;
	free(pool->queue);
	pool->queue=NULL;
	free(pool);
	pool=NULL;
	return;
}

//添加任务到队列中
void task_queue_add(pthread_pool *pool,task t){
	task_queue *queue=pool->queue;
	if(queue->count>=queue->max_size){

		perror("当前等待任务个数已满");
		return;
	}
	if(queue->rear=-1){
		queue->rear=queue->front=0;
	}else{

		queue->rear++;
	}

	queue->tasks[queue->rear]=t;
	queue->count++;
	sem_post(&pool->sem_tasks);
	return;
}

//添加任务到线程池中

void pthread_pool_add_task(pthread_pool *pool,void *(*fun)(void*),void *args){

	if(pool==NULL){

		puts("线程池未初始化");
		return;
	}
	if(pool->idleCount<=0){
		puts("当前等待线程已满");
		return;
	}
	task t;
	t.fun=fun;
	t.args=args;
	
	sem_wait(&pool->sem_workers);
	pool->idleCount--;
	task_queue_add(pool,t);
	return;
}


//获取任务
task task_queue_get(task_queue *queue){

	task t;
	if(queue==NULL || queue->count==0){

		puts("当前没有任务需要执行");
		return t;
	}

	pthread_mutex_lock(&mutex_pool);
	t=queue->tasks[queue->front];
	queue->count--;
	if(queue->count==0){

		queue->front=queue->rear=-1;
	}else{

		queue->rear++;
	}
	pthread_mutex_unlock(&mutex_pool);
	return t;

}


//线程工作函数
void *Exec(void *args){

	if(args==NULL){

		perror("当前pool为空");
		return NULL;
	}
	pthread_pool *pool=(pthread_pool *)args;
	while(true){

		sem_wait(&pool->sem_tasks);
		task t=task_queue_get(pool->queue);
		if(t.fun!=NULL){

			t.fun(t.args);
		}
		sem_post(&pool->sem_workers);
		pool->idleCount++;
	}
	if(pool->queue->count==0){

		puts("当前没有排队的任务执行");
	}
	return NULL;
}
