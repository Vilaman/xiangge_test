/***********************************************************************************
# File Name:    calculate.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月06日 星期一 18时22分59秒
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

int (*pfun)(int,int);

void calculate(int x,int y,int (*pfun)(int,int));
int Add();
int Sub();
int Mul();
int Dev();


int main(){

	int a,b;
	int chose;
	puts("请输入两个值");
	scanf(" %d %d",&a,&b);
	puts("请选择运算符");
	puts("1.Add,2.Sub,3.Mul,4.Dev");
	scanf(" %d",&chose);
	switch(chose){

		case 1:
			pfun=Add;
			break;
		case 2:
			pfun=Sub;
			break;
		case 3:
			pfun=Mul;
			break;
		case 4:
			pfun=Dev;
			break;
		default:
			puts("输入正确选项");
			break;
	}
	calculate(a,b,pfun);


    return 0;
}


int Add(int x,int y){

	return x+y;
}

int Sub(int x,int y){

	return x-y;
}

int Mul(int x,int y){

	return x*y;
}

int Dev(int x,int y){
	if(y==0){
		return 0;
	}
	return x/y;
}


void calculate(int x,int y,int (*pfun)(int,int)){

	int result=pfun(x,y);
	printf("%d\n",result);
	return;

}
