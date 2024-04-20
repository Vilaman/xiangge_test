/***********************************************************************************
# File Name:    malloc.c
# Author:       WENPINGXIANG
# Mail:         630556320@qq.com
# Created Time: 2024年04月14日 星期日 04时55分10秒
 **********************************************************************************/

#include <stdio.h>
#include<stdbool.h> 
#include<string.h> 
#include<stdlib.h> 
int main(){
	int* p=(int *)malloc(10*sizeof(int));
	for(int i=0;i<10;i++){
		printf("%d\n",p[i]);
	}
	free(p);
	p=NULL;
    return 0;
}
