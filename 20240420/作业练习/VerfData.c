/***********************************************************************************
# File Name:    VerfData.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年04月23日 星期二 10时35分25秒
 **********************************************************************************/

#include <stdio.h>
#include<string.h> 
#include<stdbool.h>
#include<stdlib.h>
#include"myList.h"
int main() {
  List list = initlist();
  Emp emp={"admin","abc","男",24};
  InsertData(&list, emp, 0);
 /* InsertData(&list, 3, 0);
  InsertData(&list, 4, 0);
  InsertData(&list, 66, 0);
  InsertData(&list, 99, 0);
  InsertData(&list, 888, 0);
  InsertData(&list, 999, 0);
  printf("插入成功后的数据\n");
  ShowallData(&list);
  printf("替换成功后的数据\n");
  ChangeData(&list, 2, 100);
  ShowallData(&list);
  printf("删除成功后的数据\n");
  DeleteData(&list, 5);*/
  ShowallData(&list);

/*  int data = 0;
  printf("请你输入要寻找的数据\n");
  scanf(" %d", &data);
  int idex = QueryData(&list, data);
  if (idex == -1) {
    printf("数据里没有查找到此数据\n");
  } else {
    printf("查找数据的下标为%d\n", idex);
  }*/

  return 0;
}
