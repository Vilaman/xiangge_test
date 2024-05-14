/***********************************************************************************
# File Name:    lianxi11.c
# Author:       WENPINGXIANG
# Mail:         630556320@qq.com
# Created Time: 2024年04月12日 星期五 06时11分37秒
 **********************************************************************************/

#include <stdio.h>
#include<stdbool.h> 
#include<stdlib.h>
#include<time.h>
int main(){
	srand((unsigned int)time(NULL));
	int min = 0;
	int max = 1000;//设置随机范围，包含
	int randomNumber = min + rand() % (max - min + 1);
	int act;
	int count=0;
	while(act!=randomNumber){
		printf("请输入你的数字\n");
		scanf(" %d",&act);
		if(act>randomNumber){
			printf("你猜大了，请重新输入：\n");
			count++;
		}else if(act<randomNumber){
			printf("你猜小了，请重新输入\n");
			count++;
		}else{
			count++;
			printf("恭喜你猜对了，您一共·猜了%d次\n",count);
			break;
		}
	}
    return 0;
}
