/***********************************************************************************
# File Name:    demo.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月06日 星期一 19时45分22秒
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


typedef struct {
  short age;//2
  char name[64];//64
  char pass[16];//16
} User; //定义一个结构体全局变量

void demo();

int main() {
	User *users = (User *)calloc(10, sizeof(User));//在堆中申请10个User大小的空间
	int a = 1;//保存在栈中的局部变量
	a = 2;//a值变为2
	short b = 3; //保存在栈中的局部变量 short型
	a += b;//此时a的值为5
	char c = 'a';  //指向常量区'a'=97
	a+=c;//强转为int型输出 5+97=102，仍然保存在栈中
	printf("%d",a);
	demo();
	demo();
	return 0;
}

void demo() {
  static char chs[] = "admin";
  int ch = (int)chs[0];//97 98
  ch++;//98 99
  chs[0] = (char)ch;//b c
  printf("%s\n",chs);//bdmin cdmin
}
