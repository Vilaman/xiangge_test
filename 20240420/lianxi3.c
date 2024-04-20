/***********************************************************************************
# File Name:    lianxi3.c
# Author:       WENPINGXIANG
# Mail:         630556320@qq.com
# Created Time: 2024年04月12日 星期五 04时44分20秒
 **********************************************************************************/

#include <stdio.h>
#include<stdbool.h> 
int main(){
	int i,sum;
	while(i<=500){
		if(i>=100){
			sum+=i;
		}
		i++;
	}
	printf("100加到500的和为：%d\n",sum);
    return 0;
}
