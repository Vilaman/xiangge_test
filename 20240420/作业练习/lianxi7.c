/***********************************************************************************
# File Name:    lianxi7.c
# Author:       WENPINGXIANG
# Mail:         630556320@qq.com
# Created Time: 2024年04月12日 星期五 05时05分10秒
 **********************************************************************************/

#include <stdio.h>
#include<stdbool.h> 
int main(){
	int i;
	while(i<=999){
		if(i>=100){
			int n1=i%10;
			int n2=i/10%10;
			int n3=i/100;
			if(i==n1*n1*n1+n2*n2*n2+n3*n3*n3){
				printf("%d是水仙花数\n",i);
			}
		}
		i++;
	}
    return 0;
}
