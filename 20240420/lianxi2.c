/***********************************************************************************
# File Name:    lianxi2.c
# Author:       WENPINGXIANG
# Mail:         630556320@qq.com
# Created Time: 2024年04月12日 星期五 04时40分47秒
 **********************************************************************************/

#include <stdio.h>
#include<stdbool.h> 
int main(){
	int i,sum;
	while(i<=10){
		sum+=i;
		i++;
	}
	printf("1到10的和为：%d\n",sum);
    return 0;
}
