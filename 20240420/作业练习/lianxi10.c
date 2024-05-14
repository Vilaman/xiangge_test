/***********************************************************************************
# File Name:    lianxi10.c
# Author:       WENPINGXIANG
# Mail:         630556320@qq.com
# Created Time: 2024年04月12日 星期五 06时04分33秒
 **********************************************************************************/

#include <stdio.h>
#include<stdbool.h> 
int main(){
	int i=1;
	while(i<=100){
		int j=1;
		int count=0;
		while(j<=i){
			if(i%j==0){
				count++;
			}
			j++;
		}
		if(count==2){
			printf("%d是质数\n",i);
		}
		i++;
	}
    return 0;
}
