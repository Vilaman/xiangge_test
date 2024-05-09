/***********************************************************************************
# File Name:    线程池.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月09日 星期四 16时29分07秒
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



//定义线程任务结构体
typedef struct task {
  void *(*fun)(void *);
  void *args;
} task;


//定义任务结构体队列
typedef struct queue{
	int front;
	int rear;
	int count;//队列添加的要执行的任务个数
	int max_size;//队列能添加的最大排队个数
	task *tasks;//指向任务数组的第一个地址  需要添加的任务
}task_queue;

//定义线程池结构体
typedef struct pool{
	pthread_t *pids;//线程数组  
	int threadCount;//线程数量  池子最大处理的线程数量
	sem_t sem_tasks;//用于控制任务队列互斥 实际是 排队要执行的任务个数
	sem_t sem_workers;//用于控制线程数量 就是当前空闲线程数
	task_queue *queue;//任务队列结构体指针  通过队列来控制任务执行 先来先执行
}pthread_pool;



int main(){

    return 0;
}

//初始化线程池->需要传一个需要初始化的pool指针进去，池子能处理的最大线程数
//队列能添加的最大排队个数
void pool_init(pthread_pool *pool,int thread_count,int queue_size){

	pool->threadCount=thread_count;
	//设置要创建同时处理的线程数组
	pthread_t *pids=malloc(sizeof(pthread_t)*thread_count);
	//创建信号量因为当前的任务为0所以task初始化为0；
	sem_init(&pool->sem_tasks,0,0);
	sem_init(&pool->sem_workers,0,thread_count);

//都是指针所以把pid申请的空间直接给它
	pool->pids=pids;

	//定义一个task的结构体指针 方便初始化
	task_queue *queue=malloc(sizeof(task_queue));
	pool->queue=queue;

//调用task队列结构初始化
	task_queue_init(queue,queue_size);
	//创建线程个数为pool处理的最大个数
	for(int i=0;i<thread_count;i++){

		pthread_create(&pids[i],NULL,worker,pool);
	}

	return;
}


//任务队列初始化
void task_queue_init(task_queue *queue,int max_size){
	if(queue==NULL){

		perror("queue为空无法初始化");
		exit(1);
	}

	//给队列中任务分配空间及task结构体
	queue->tasks=(task *)malloc(sizeof(task)*max_size);
	queue->rear=queue->front=-1;//因为此时队列中没有数据因此首尾下标都为-1
	queue->count=0;
	queue->max->size=max_size;
	return;
}


//添加新任务
void pool_add_task(pthread_pool *pool,void *(*fun)(void *),void *args){
	//如果传入的pool指针就来为空就表示线程池没有初始化
	if(pool==NULL){
		perror("线程池未初始化");
		return;
	}
	task t;//定义一个任务结构体变量
	t.fun=fun;
	t.args=args;
	//用信号量来控制是否有空闲的线程并且让空闲线程sem_workers -1
	sem_wait(&pool->sem_workers);
	//调用队列任务添加到任务队列中
	task_queue_add(pool,t);
	return;
}

//添加任务到排队队列中
void task_queue_add(pthread_pool *pool,task t){

	task_queue *queue=pool->queue;//定义一个task结构体指针等于pool中初始化的指针
	//当队列中当前数量大于或者等于最大能排队的个数时无法入队
	if(queue->count>=queue->max_size){

		printf("队列已满，无法入队\n");
		return;
	}
//如果当前队列中没有在排队的任务时
	if(queue->rear==-1){
		queue->front=queue->rear=0;
	}else{

		queue->rear++;
	}
	queue->tasks[queue-rear]=task;
	//任务个数+1
	queue->count++;
	//任务量+1  及要执行的任务个数
	sem_post(&pool->sem_tasks);
	return;
}

//从排队任务队列中获取任务

task task_queue_get(task_queue *queue){

	task t;
	//如果当前队列为NULL说明没有任务需要执行或者未初始化
	if(queue==NULL || queue->count==0){

		puts("当前没有任务需要执行");
		return t;//直接返回空结构体
	}
	//锁定当前任务避免其他线程抢时间片去执行
	pthread_mutex_lock(&mutex);
	t=queue->tasks[queue->front];
	queue->count--;//将队列中的排队数-1
//如果队列中排队数为0时则表示当前没有排队的任务
	if(queue->count==0){
		queue->front=queue->rear=-1;
	}else{

		queue->rear++;
	}
	pthread_mutex_unlock(&mutex);
	return t;
}
