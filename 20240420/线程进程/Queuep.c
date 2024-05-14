/***********************************************************************************
# File Name:    Queuep.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年04月23日 星期二 16时31分22秒
 **********************************************************************************/

#include <stdio.h>
#include<string.h> 
#include<stdbool.h>
#include<stdlib.h> 
#include"myQueue.h"


int main(){

Queue q=initQueue();
EnterQueue(&q,39);
EnterQueue(&q,99);
EnterQueue(&q,69);
EnterQueue(&q,59);
EnterQueue(&q,9);
EnterQueue(&q,88);
PrintQueue(&q);
printf("--------------------\n");
DeleteQueue(&q);
EnterQueue(&q,18);
EnterQueue(&q,88);
PrintQueue(&q);


    return 0;
}
