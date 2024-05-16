/***********************************************************************************
# File Name:    pthread_pool.h
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月10日 星期五 09时13分55秒
 **********************************************************************************/

#ifndef PTHREAD_POOL_H
#define PTHREAD_POOL_H

typedef struct task{
	void *(*fun)(void *);
	void *args;
}task;


typedef struct queue{

	int front;
	int rear;
	int count;
	int max_size;
	task *tasks;
}task_queue;

typedef struct pool{
	int idleCount;
	pthread_t *pids;
	int threadCount;
	sem_t sem_tasks;
	sem_t sem_workers;
	task_queue *queue;
}pthread_pool;




void pool_init(pthread_pool *pool,int thread_max,int queue_max);
void pthread_pool_add_task(pthread_pool *pool,void *(*fun)(void *),void *args);
void task_queue_add(pthread_pool *pool,task t);
task task_queue_get(task_queue *queue);
void *Exec(void *args);
void pthread_pool_destroy(pthread_pool *pool);





#endif
