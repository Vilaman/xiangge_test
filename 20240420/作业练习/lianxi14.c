/***********************************************************************************
# File Name:    lianxi14.c
# Author:       WENPINGXIANG
# Mail:         630556320@qq.com
# Created Time: 2024年04月12日 星期五 07时39分33秒
 **********************************************************************************/

#include <stdio.h>
#include<stdbool.h> 
int main() {
    int i = 0;
	int count=0;
    while(i <= 50) {
		int j=0;
		while(j<=50){
		int	h=0;
			while(h<13){
				if(i+j+h==50 && i+j*2+h*5==100){
					count+=1;
					printf("一分：%d, 两分：%d, 5分: %d\n",i,j,h);
				}
				h+=1;
				}
			j+=1;
		}
		i+=1;
		
	}
	printf("组合方案=%d种\n",count);
	return 0;
}
