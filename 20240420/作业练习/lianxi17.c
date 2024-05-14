/***********************************************************************************
# File Name:    lianxi17.c
# Author:       WENPINGXIANG
# Mail:         630556320@qq.com
# Created Time: 2024年04月12日 星期五 18时05分51秒
 **********************************************************************************/

#include <stdio.h>
#include<stdbool.h> 
int main(){
	int num;
	int max=0;
	int min=0;
	printf("请输入一个整数：\n");
	scanf(" %d",&num);
	while(num!=0){
		printf("请继续输入一个整数按0退出：\n");
		scanf(" %d",&num);
		if(max==0){
			max=num;
			min=num;
		}
		else if(num>max){
			max=num;
		}
		else if(num<min){
			min=num;
		}
		
	}
	printf("最大值为:%d,最小值为:%d\n",max,min);
    return 0;
}
