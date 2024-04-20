/***********************************************************************************
# File Name:    lianxi8.c
# Author:       WENPINGXIANG
# Mail:         630556320@qq.com
# Created Time: 2024年04月12日 星期五 05时16分37秒
 **********************************************************************************/

#include <stdio.h>
#include<stdbool.h> 
int main(){
	int i=0;
	while(i<=999){
		if(i>=100){
			int n1=i/100;
			int n2=i /10 % 10;
			int n3=i % 10;
			if(n1==n2 && n1==n3){
				printf("%d是所有位数相同的数字\n",i);
			}
		}
		i++;
	}
    return 0;
}
