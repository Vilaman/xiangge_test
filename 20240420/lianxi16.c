/***********************************************************************************
# File Name:    lianxi16.c
# Author:       WENPINGXIANG
# Mail:         630556320@qq.com
# Created Time: 2024年04月12日 星期五 09时24分28秒
 **********************************************************************************/

#include <stdio.h>
#include<stdbool.h> 
int main(){
	int num=1;
	while(num!=0){
		printf("请输入大于1的行数或者按0退出\n");
		scanf(" %d",&num);
		int i=1;
		for(i=1;i<=num;i++){
			int j=1;
			for(j=1;j<=num-i;j++){
				printf(" ");
			}
			int k=1;
			for(k=1;k<=2*i-1;k++){
				printf("*");
			}
			printf("\n");
		}
		for(int x=1;x<=num-1;x++){
			for(int j=1;j<=x;j++){
				printf(" ");
			}
			for(int k=1;k<=2*(num-x)-1;k++){
				printf("*");
			}
			printf("\n");
		
		}
		
	}
	
    return 0;
}
