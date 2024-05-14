/***********************************************************************************
# File Name:    日志写入.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月07日 星期二 19时57分20秒
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
int main(){

    return 0;
}

void writetoLog(char *info){

	pthread_t ppid=pthread_self();
	pthread_detach(ppid);
	FILE *pf=fopen("connectlog.txt","a+");
	if(pf==NULL){
		perror("fopen");
		exit(2);
	}
	char status[]="login";
	time_t t=time(NULL);
	struct tm *now=localtime(&t);
	char timestr[40];
	strftime(timestr,sizeof(timestr)-1,"%y年%m月%d日 %H:%M:%S",now);
	timestr[strlen(timestr)]='\0';
	fprintf(pf,"%s %s %s\n",info,timestr,status);
	puts("写入日志成功");
	fclose(pf);
	pf=NULL;
}

