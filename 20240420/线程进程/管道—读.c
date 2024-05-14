/***********************************************************************************
# File Name:    管道—读.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月11日 星期六 12时15分16秒
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


char path[]="/tmp/my_fifo2";

int main(){

	int ret=mkfifo(path,0666);
	if(ret<0){

		perror("mkfifo");
		exit(1);
	}
	int fp=open(path,O_RDONLY);
	if(fp<0){

		perror("open()");
		exit(1);
	}
	char data[10];
	int readcount=read(fp,data,sizeof(data)-1);
	if(readcount>0){

		data[readcount]='\0';
		puts(data);
	}

	close(fp);
    return 0;
}

