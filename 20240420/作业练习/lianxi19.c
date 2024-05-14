/***********************************************************************************
# File Name:    lianxi19.c
# Author:       WENPINGXIANG
# Mail:         630556320@qq.com
# Created Time: 2024年04月12日 星期五 19时34分52秒
 **********************************************************************************/

#include <stdio.h>
#include<stdbool.h> 
int main(){
	int total=1;
	int sum=1;
	while(total!=0){

		sum*=2;
//		printf("sum%d\n",sum);
		total+=sum;
		if(total*6>=100){
			total-=sum;
			break;
		}
	}
	printf("购买总金额不超过100的最大苹果总数量:%d\n",total);

	return 0;
}
