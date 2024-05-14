/***********************************************************************************
# File Name:    消息队列1.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月11日 星期六 15时19分07秒
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
#include<sys/ipc.h>
#include<sys/msg.h>

typedef struct msgbuf{

	long mtype;
	char mtext[200];
}ms;



int main(){
	//定义一个消息缓冲区结构体
	ms buf;
	memset(&buf,0,sizeof(buf));
	//定义一个消息队列标识符
	int msqid;
	//定义一个键值
	key_t key;
	//使用ftok函数生成键值
	if((key=ftok("fifotest",'B'))==-1){
//与发送方使用同样的key
		perror("ftok");
		exit(1);
	}

	//使用msgget函数连接到消息队列
	if((msqid=msgget(key,0644 | IPC_CREAT))==-1){
		perror("msgget()");
		exit(1);
	}

	//写一条type为2的消息
	strcpy(buf.mtext,"helloloo");
	buf.mtype=2;
	msgsnd(msqid,&buf,strlen(buf.mtext)+1,0);

	//提示
	printf("消息发出type为2\n");


    return 0;
}
