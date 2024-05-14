/***********************************************************************************
# File Name:    lianxi16while.c
# Author:       WENPINGXIANG
# Mail:         630556320@qq.com
# Created Time: 2024年04月13日 星期六 01时27分31秒
 **********************************************************************************/
#include <stdio.h>
#include <stdbool.h>
int main(){
    int n ;
	printf("请输入行数\n");
	scanf("%d",&n);
	int i =1;
	while(i<=n){
		int space =1;
		while(space <=n -i ){
			printf(" ");
			space++;
		}
		int j = 1;
		while(j <=2*i -1){
			printf("*");
			j++;
		}
		i++;
		printf("\n");
	}
	int j=1;
	while(j<=n-1){
		int space=1;
		while(space<=j){
			printf(" ");
			space++;
		}
		int k=1;
		while(k<=2*(n-j)-1){
				printf("*");
				k++;
				}
		j++;
		printf("\n");
	}
	return 0;
}
