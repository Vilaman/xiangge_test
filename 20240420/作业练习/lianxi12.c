/***********************************************************************************
# File Name:    lianxi12.c
# Author:       WENPINGXIANG
# Mail:         630556320@qq.com
# Created Time: 2024年04月12日 星期五 06时25分52秒
 **********************************************************************************/

#include <stdio.h>
#include<stdbool.h> 
int main(){
	int i=0,j=0;
	while(i<=15){
		j=15-i;
		if((i*2+j*4)==40){
			printf("鸡的数量%d，兔的数量%d\n",i,j);
		}
		i++;
	}
    return 0;
}
