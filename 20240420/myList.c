/***********************************************************************************
# File Name:    myList.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年04月23日 星期二 10时28分37秒
 **********************************************************************************/

#include <stdio.h>
#include<string.h> 
#include<stdbool.h>
#include<stdlib.h>
#include"myList.h"

//初始化list
List initlist() {
  List list = {0}; //定义list
  //动态申请SIZE个int空间
  Emp *space = (Emp *)malloc(SIZE * sizeof(Emp));
  if (space == 0) {
    printf("空间申请失败\n");
    exit(0);
  }
  list.arr = space;
  list.lenth = 0;
  list.capacity = SIZE;
  return list;
}
//插入数据
bool InsertData(List *list, Emp emp, int index) {
  if (index < 0 || index >= list->capacity) {
    printf("非法插入数据\n");
    return false;
  } //判断插入是否合法
  if (list->lenth == list->capacity) {
    Addcapacity(list);
  } //判断容量是否足够
  if (index >= list->lenth) {
    list->arr[list->lenth]=emp;
    list->lenth += 1;
    return true;
  } //如果index大于或等于lenth就在末尾插入数据
  for (int i = list->lenth; i > index; i--) {
    list->arr[i] = list->arr[i - 1];
  } // index小于lenth
  list->arr[index] = emp;
  list->lenth += 1;
  return true;
}
//删除数据
bool DeleteData(List *list,int index){
	if(index<0||index>=list->capacity){
		return false;
	}
	for(int i=index;i<list->lenth-1;i++){

		list->arr[i]=list->arr[i+1];
	}
	memset(&list->arr[list->lenth-1],0,sizeof(Emp));//最后一个位置置零
	list->lenth-=1; //lenth减一
	return true;
}

//修改数据
bool ChangeData(List *list,int index,Emp emp){
	if(index<0||index>=list->capacity){
		return false;
	}
	list->arr[index]=emp;
	return true;
}
//查找返回下标
int QueryData(List *list,Emp emp) {
  int index = -1;
  for (int i = 0; i < list->lenth; i++) {
    if (strcmp(list->arr[i].username,emp.username)==0) {
      index = i;
      return index;
    }
  }
  return -1;
}

//扩容
void Addcapacity(List *list) {
  list->capacity *= 2;
  Emp *newspace = (Emp *)realloc(list->arr,list->capacity * sizeof(Emp));
  //for (int i = 0; i < list->lenth; i++) {
   // newspace[i] = list->arr[i];
 // } //复制旧数据到新空间
  free(list->arr);
  list->arr = newspace;
  printf("扩容成功，当前容量为%d\n", list->capacity);
  return;
}


//查看所有数据
void ShowallData(List *list) {
  for (int i = 0; i < list->lenth; i++) {
    printf("下标为%d的数据为%s,密码为%s,性别为%s,年龄为%d\n",i, list->arr[i].username,list->arr[i].password,list->arr[i].sex,list->arr[i].age);
  }
}

