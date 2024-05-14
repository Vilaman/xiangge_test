/***********************************************************************************
# File Name:    lianxi13.c
# Author:       WENPINGXIANG
# Mail:         630556320@qq.com
# Created Time: 2024年04月12日 星期五 07时23分49秒
 **********************************************************************************/

#include <stdio.h>
#include<stdbool.h>
void mypower(int x,int y){
	int i=1;
	int sum=1;
	while(i<=y){
		sum*=x;
		i++;
	}
	printf("%d的%d次方是：%d\n",x,y,sum);
}

int main(){
	int x,y;
	printf("请输入数字及要求的幂\n");
	scanf(" %d %d",&x,&y);
	mypower(x,y);

    return 0;
}
