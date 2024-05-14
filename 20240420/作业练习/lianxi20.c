/***********************************************************************************
# File Name:    lianxi20.c
# Author:       WENPINGXIANG
# Mail:         630556320@qq.com
# Created Time: 2024年04月13日 星期六 00时29分16秒
 **********************************************************************************/

#include <stdio.h>
#include<stdbool.h> 
int main(){
	int money=20;
	int b;
	int sum=0;
	while(money>=3){
		b=money/3;
		money=money%3+money/3;
		sum+=b;
	}
	printf("可以喝到%d瓶可乐\n",sum);
    return 0;
}
