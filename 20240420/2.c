/***********************************************************************************
# File Name:    2.c
# Author:       WENPINGXIANG
# Mail:         630556320@qq.com
# Created Time: 2024年04月19日 星期五 06时03分23秒
 **********************************************************************************/

#include <stdio.h>
#include<stdbool.h> 
#include<string.h> 
#include<stdlib.h> 

typedef struct user {
  char name[16];
  int age;
  char sex[5];
}u;

u *users; //后续分配空间，避免野指针，声明为空指针
int main(){
users=(u *)malloc(5*sizeof(u));
printf("%d\n",sizeof(*users));
  return 0;
}
