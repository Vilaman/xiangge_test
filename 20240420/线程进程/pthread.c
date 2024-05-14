/***********************************************************************************
# File Name:    pthread.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月07日 星期二 11时32分04秒
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

void *demo(void *args);
void *demo1(void *args1);

int main(){
	pthread_t pid1;
	pthread_t pid2;
	int args=3;
	char *args1="aada";
	int back=pthread_create(&pid1,NULL,demo,&args);
	if(back!=0){

		perror("pthread_create");
		exit(1);
	}else{

		puts("线程1创建成功");
	}
	int back2=pthread_create(&pid2,NULL,demo1,args1);
	if(back2!=0){

		perror("pthread_create");
		exit(1);
	}else{

		puts("线程2创建成功");
	}
	pthread_join(pid1,NULL);
	pthread_join(pid2,NULL);




    return 0;
}

void *demo(void *args){

	puts("线程1执行代码");
	printf("线程1接收到的数据是%d\n",*(int *)args);
}


void *demo1(void *args1){

	puts("线程2执行代码");
	printf("线程2接收到的数据是%s\n",(char *)args1);
}
