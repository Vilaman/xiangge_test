/***********************************************************************************
# File Name:    管道—父子进程.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月11日 星期六 14时18分04秒
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
#include<sys/stat.h>

char path[]="./fifo1";

int main(){
	
	char data[20];
	char input[19]="helloqwq";
	int pid=fork();
	mkfifo(path,0666);
	if(pid==0){
		int fd=open(path,O_RDONLY);
		if(fd<0){
			perror("open");
			exit(1);
		}
		int readcount=read(fd,data,sizeof(data)-1);
		if(readcount>0){
			data[readcount]='\0';
			puts(data);
		}
		close(fd);
	}else if(pid>0){

		int pf=open(path,O_WRONLY);
		write(pf,input,sizeof(input));
//	wait(NULL);
		close(pf);
	}

    return 0;
}
