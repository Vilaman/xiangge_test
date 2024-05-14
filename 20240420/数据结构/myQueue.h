/***********************************************************************************
# File Name:    myQueue.h
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年04月23日 星期二 15时45分32秒
 **********************************************************************************/
// 在于防止多次引入头文件时导致重复定义
#ifndef MYQUEUE_H
#define MYQUEUE_H
#define SIZE 10

typedef struct Emp{
	char username[16];
	char password[16];
	char sex[5];
	int age;
}Emp;//设置别名emp


typedef struct Queue {
  Emp *head;
  int front;
  int rear;
} Queue;


Queue initQueue();
void EnterQueue();
void DeleteQueue();
void PrintQueue();
bool isQueueFull();
bool isQueueEmpty();
void moveQueue();













#endif
