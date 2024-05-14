/***********************************************************************************
# File Name:    lianxi9.c
# Author:       WENPINGXIANG
# Mail:         630556320@qq.com
# Created Time: 2024年04月12日 星期五 05时39分37秒
 **********************************************************************************/

#include <stdio.h>
#include<stdbool.h> 
int main(){
	int num;
	printf("请输入一个数\n");
	scanf(" %d",&num);
	int i=1;
	int count=0;
	while(i<=num){
		if(num%i==0){
			count++;
		}
		i++;
	}
	if(count==2){
		printf("%d是一个质数\n",num);
	}else{
		printf("%d不是一个质数\n",num);
	}

    return 0;
}
