/***********************************************************************************
# File Name:    lianxi15.c
# Author:       WENPINGXIANG
# Mail:         630556320@qq.com
# Created Time: 2024年04月12日 星期五 09时13分53秒
 **********************************************************************************/

#include <stdio.h>
#include<stdbool.h> 
int main(){
	int red=0;
	int count=0;
	while(red<=5){
		int black=0;
		while(black<=7){
			int white=0;
			while(white<=9){
				if(red+black+white==12){
					count+=1;
					printf("红球有%d个,黑球有%d个，白球有%d个\n",red,black,white);
				}
				white+=1;
			}
			black+=1;
		}
		red+=1;
	}

	printf("所有取球方式有%d种\n",count);
    return 0;
}
