/***********************************************************************************
# File Name:    test1.c
# Author:       WENPINGXIANG
# Mail:         630556320@qq.com
# Created Time: 2024年05月10日 星期五 07时02分45秒
 **********************************************************************************/

#include <stdio.h>
#include<stdbool.h> 
#include<string.h> 
#include<stdlib.h> 
#include <semaphore.h>
 
int main() {
    sem_t sem;
    int value;
    
    // 初始化信号量，初始值为1
    if(sem_init(&sem, 0, 3) != 0) {
        perror("sem_init");
        return 1;
    }
    
    // 获取信号量的值
    if(sem_getvalue(&sem, &value) != 0) {
        perror("sem_getvalue");
        return 1;
    }
    
    printf("The value of the semaphore is %d\n", value);
    
    // 操作结束，销毁信号量
    if(sem_destroy(&sem) != 0) {
        perror("sem_destroy");
        return 1;
    }
    
    return 0;
}
