/***********************************************************************************
# File Name:    lianxi6.c
# Author:       WENPINGXIANG
# Mail:         630556320@qq.com
# Created Time: 2024年04月12日 星期五 04时58分43秒
 **********************************************************************************/

#include <stdio.h>
#include<stdbool.h> 
int main(){
	int i;
	while(i<=100){
		if(i%5==0&&i!=0){
			printf("%d能被5整除\n",i);
		}
		i++;	
	}
    return 0;
}
