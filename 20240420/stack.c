/***********************************************************************************
# File Name:    stack.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年04月24日 星期三 11时39分40秒
 **********************************************************************************/

#include <stdio.h>
#include<string.h> 
#include<stdbool.h>
#include<stdlib.h> 
typedef int stacktype;

typedef struct Stack{
stacktype *head;
int top;
int capacity;
}Stack;


Stack initstack();
void Instack(Stack *s,stacktype data);
stacktype Delstack(Stack *s);
bool isStackFull(Stack *s);
bool isStackEmpty(Stack *s);
void ShowStackData(Stack *s);

Stack c;

int main(){
	c=initstack(5);
	Instack(&c,20);
	Instack(&c,30);
	Instack(&c,40);
	Instack(&c,50);
	Instack(&c,60);
	Instack(&c,70);
	ShowStackData(&c);
	Delstack(&c);
	Delstack(&c);
	ShowStackData(&c);
    return 0;
}


//初始化
Stack initstack(int size) {
  Stack s;
  stacktype *space = (stacktype *)malloc(size * sizeof(stacktype));
  if (space == NULL) {
    exit(-1);
  }
  s.head = space;
  s.top = -1;
  s.capacity = size;
  return s;
}

//入栈
void Instack(Stack *s, stacktype data) {
  if (isStackFull(s)) {
    printf("空间已满\n");
    return;
  }
  s->head[++s->top] = data;
  return;
}

//出栈
stacktype Delstack(Stack *s){

	if(isStackEmpty(s)){
		printf("栈空间为空\n");
		return -1;
	}
	return s->head[s->top--];
}


//判断空间为满
bool isStackFull(Stack *s) { 
	return s->top == s->capacity - 1;
}

//判断空间为空
bool isStackEmpty(Stack *s){
	return s->top==-1;

}

//查看栈内所有数据
void ShowStackData(Stack *s) {
  for (int i = 0; i <= s->top; i++) {

    printf("%d\n", s->head[i]);
  }
  return;
}

