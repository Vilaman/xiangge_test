/***********************************************************************************
# File Name:    zhizhen6.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年04月22日 星期一 11时48分06秒
 **********************************************************************************/

#include <stdio.h>
#include<string.h> 
#include<stdbool.h>
#include<stdlib.h> 
int *print(int (*arr)[3]){

	return arr[2];
}


int main(){
	int arr[][3]={1,2,3,4,5,6,7,8,9};
	int *a=print(arr);
	printf("%d\n",*(a+1));
    return 0;
}
