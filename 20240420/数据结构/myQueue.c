/***********************************************************************************
# File Name:    myQueue.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年04月23日 星期二 16时01分05秒
 **********************************************************************************/

#include <stdio.h>
#include<string.h> 
#include<stdbool.h>
#include<stdlib.h>
#include"myQueue.h"

Queue initQueue() {
  Queue q;
  Emp *space = (Emp *)malloc(SIZE * sizeof(Emp));
  if (space == NULL) {
    exit(0);
  }
  q.head=space;
  q.front=q.rear=-1;
  return q;
}

//插入数据
void EnterQueue(Queue *q, Emp emp) {
  moveQueue(q);
  if (isQueueFull(q)) {
    printf("插入失败\n");
    return;
  }//如果当前队列没有值则将队头设为0下标
  if (q->front == -1) {

    q->front = 0;
  }
  q->rear+=1;
  q->head[q->rear]=emp;
}


//删除数据
void DeleteQueue(Queue *q) {
  if ( isQueueEmpty(q)) {
    printf("当前为空\n");
    exit(-1);
  }
  //如果当前只有一个数据
  if (q->front == q->rear) {
 //   emp1 = q->head[q->front]; 
	memset(&q->head[q->front],0,sizeof(Emp));
    q->front = -1;
    q->rear = -1;
  } else {
   // emp1 = q->head[q->front];
	memset(&q->head[q->front],0,sizeof(Emp));
    q->front += 1;
  }
  return;
}



void PrintQueue(Queue *q) {
  for (int i = q->front; i <= q->rear; i++) {
    printf("下标为%d的用户为%s，密码为%s，性别为%s，年龄为%d\n", i,
           q->head[i].username, q->head[i].password, q->head[i].sex,
           q->head[i].age);
  }
}


//判断空间是否已满
bool isQueueFull(Queue *q){

	return q->rear==SIZE-1;
}


//判断是否为空
bool isQueueEmpty(Queue *q){

	return q->front==-1;
}


//删除列之后让队头归零避免假溢出
void moveQueue(Queue *q) {
  if (q->front <= 0 || q->rear < SIZE - 1) {

    return;
  }
  int diffval = q->rear - q->front;
  for (int i = 0; i <= diffval; i++) {
    q->head[i] = q->head[q->front+ i];
  }
  q->front = 0;
  q->rear = diffval;
  printf("移动了数据%d,%d\n",q->front,q->rear);
	  
}
