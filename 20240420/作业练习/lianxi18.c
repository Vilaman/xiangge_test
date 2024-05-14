/***********************************************************************************
# File Name:    lianxi18.c
# Author:       WENPINGXIANG
# Mail:         630556320@qq.com
# Created Time: 2024年04月12日 星期五 19时14分48秒
 **********************************************************************************/

#include <stdio.h>
#include<stdbool.h> 
int main(){
	int i=100;
	while(i<999){
		int bai=i/100;
		int shi=i/10%10;
		int ge=i%10;
		if(bai+shi+ge==6){
			printf("%d\n",i);
		}
		i+=1;
	}
    return 0;
}
