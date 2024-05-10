/***********************************************************************************
# File Name:    父子进程.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月10日 星期五 15时12分22秒
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

	printf("main进程\n");
	int pid=fork();

	if(pid>0){
//父进程
	printf("父进程\n");
	printf("父进程:pid%d\n",getpid());
	int status;
	//等待子进程结束
	waitpid(pid,&status,0);
	int num=WEXITSTATUS(status);
	printf("子进程结束传给exit()的状态码%d\n",num);
	}else if(pid==0){

		int child_pid=getpid();
		printf("子进程:%d\n",child_pid);
		printf("子进程输出给父进程的ID%d\n",getpid());
		exit(1);
	}else{

		perror("fork()");
	}
    return 0;
}
