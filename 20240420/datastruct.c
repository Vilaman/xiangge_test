/***********************************************************************************
# File Name:    datastruct.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年04月22日 星期一 19时09分43秒
 **********************************************************************************/

#include <stdio.h>
#include<string.h> 
#include<stdbool.h>
#include<stdlib.h>
typedef struct List{
	int *arr; //指向列表的第一个数据
	int lenth; //列表当前数据个数
	int capacity; //列表当前总容量
}List;
//初始容量为5
int size=5;
List initlist();
void ShowallData(List *list);
void Addcapacity(List *list);
int QueryData(List *list, int data);
bool ChangeData(List *list,int index,int data);
bool DeleteData(List *list,int index);
bool InsertData(List *list,int data,int index);


int main(){
	List list=initlist();
	InsertData(&list,2,0);
	InsertData(&list,3,0);
	InsertData(&list,4,0);
	InsertData(&list,66,0);
	InsertData(&list,99,0);
	InsertData(&list,888,0);
	InsertData(&list,999,0);
	printf("插入成功后的数据\n");
	ShowallData(&list);
	printf("替换成功后的数据\n");
	ChangeData(&list,2,100);
	ShowallData(&list);
	printf("删除成功后的数据\n");
	DeleteData(&list,5);
	ShowallData(&list);

	int data=0;
	printf("请你输入要寻找的数据\n");
	scanf(" %d",&data);
	int idex=QueryData(&list,data);
	if(idex==-1){
		printf("数据里没有查找到此数据\n");
	}else{
		printf("查找数据的下标为%d\n",idex);
	}


    return 0;

}


//初始化list
List initlist() {
  List list = {0}; //定义list
  //动态申请5个int空间
  int *space = (int *)malloc(size * sizeof(int));
  if (space == 0) {
    printf("空间申请失败\n");
    exit(0);
  }
  list.arr = space;
  list.lenth = 0;
  list.capacity = size;
  return list;
}
//插入数据
bool InsertData(List *list, int data, int index) {
  if (index < 0 || index >= list->capacity) {
    printf("非法插入数据\n");
    return false;
  } //判断插入是否合法
  if (list->lenth == list->capacity) {
    Addcapacity(list);
  } //判断容量是否足够
  if (index >= list->lenth) {
    list->arr[list->lenth] = data;
    list->lenth += 1;
    return true;
  } //如果index大于或等于lenth就在末尾插入数据
  for (int i = list->lenth; i > index; i--) {
    list->arr[i] = list->arr[i - 1];
  } // index小于lenth
  list->arr[index] = data;
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
	list->arr[list->lenth-1]=0;//最后一个位置置零
	list->lenth-=1; //lenth减一
	return true;
}

//修改数据
bool ChangeData(List *list,int index,int data){
	if(index<0||index>=list->capacity){
		return false;
	}
	list->arr[index]=data;
	return true;
}
//查找返回下标
int QueryData(List *list, int data) {
  int index = -1;
  for (int i = 0; i < list->lenth; i++) {
    if (list->arr[i] == data) {
      index = i;
      return index;
    }
  }
  return -1;
}

//扩容
void Addcapacity(List *list) {
  list->capacity *= 2;
  int *newspace = (int *)malloc(list->capacity * sizeof(int));
  for (int i = 0; i < list->lenth; i++) {
    newspace[i] = list->arr[i];
  } //复制旧数据到新空间
  free(list->arr);
  list->arr = newspace;
  printf("扩容成功，当前容量为%d\n", list->capacity);
  return;
}


//查看所有数据
void ShowallData(List *list) {
  for (int i = 0; i < list->lenth; i++) {
    printf("下标为%d的数据为%d\n",i, list->arr[i]);
  }
}
