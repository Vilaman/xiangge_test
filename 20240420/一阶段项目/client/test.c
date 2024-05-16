/***********************************************************************************
# File Name:    test.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月16日 星期四 18时41分49秒
 **********************************************************************************/

#include <stdio.h>
#include<string.h> 
#include<stdbool.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<pthread.h>
#include<semaphore.h>
#include<sys/wait.h>
#include<sys/msg.h>
#include<sqlite3.h>
int main(){
char str[] = "Hello,World";
    char *token;
	char *t2;

    // 获取第一个片段
    token = strtok(str, ",");
    t2=strtok(NULL,",");
    // 继续获取后续片段
	printf("%s,%s",token,t2);
    return 0;
}
