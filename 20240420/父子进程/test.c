/***********************************************************************************
# File Name:    test.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月11日 星期六 09时35分46秒
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
int main(){

	char *ip=malloc(sizeof(char));
	printf("ip地址%p\n",ip);
	free(ip);
	printf("ip地址%p\n",ip);

    return 0;
}
