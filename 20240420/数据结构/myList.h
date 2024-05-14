/***********************************************************************************
# File Name:    myList.h
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年04月23日 星期二 09时55分06秒
 **********************************************************************************/

// 在于防止多次引入头文件时导致重复定义
#ifndef MYLIST_H
#define MYLIST_H
//初始容量
#define SIZE 10
typedef struct Emp{
	char username[16];
	char password[16];
	char sex[5];
	int age;
}Emp;


typedef struct List{
	Emp *arr; //指向列表的第一个数据
	int lenth; //列表当前数据个数
	int capacity; //列表当前总容量
}List;



List initlist();
void ShowallData(List *list);
void Addcapacity(List *list);
int QueryData(List *list, Emp);
bool ChangeData(List *list,int index,Emp);
bool DeleteData(List *list,int index);
bool InsertData(List *list,Emp,int index);



#endif
