/***********************************************************************************
# File Name:    lianxi21.c
# Author:       WENPINGXIANG
# Mail:         630556320@qq.com
# Created Time: 2024年04月13日 星期六 01时04分14秒
 **********************************************************************************/

#include <stdio.h>
#include<stdbool.h> 
int main(){
	int n,sum=0,total=0;
	printf("请输入n的值\n");
	scanf(" %d",&n);
	while(n>=0){
		sum=n*n;
		total=total+sum;
		n-=1;
	}
	printf("平方和为%d\n",total);
    return 0;
}
