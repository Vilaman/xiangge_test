/***********************************************************************************
# File Name:    lianxi4.c
# Author:       WENPINGXIANG
# Mail:         630556320@qq.com
# Created Time: 2024年04月12日 星期五 04时49分39秒
 **********************************************************************************/

#include <stdio.h>
#include<stdbool.h> 
int main(){
	int i=1;
	int sum=1;
	while(i<10){
		sum*=i;
		i++;
	}
	printf("1乘到10的积 %d\n",sum);
    return 0;
}
