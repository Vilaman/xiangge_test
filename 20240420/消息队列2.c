/***********************************************************************************
# File Name:    消息队列2.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月11日 星期六 16时22分34秒
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
#include<sys/ipc.h>

typedef struct msbuf {

  long tpye;
  char text[200];
} msbuf;

int main() {
  msbuf buf;

  memset(&buf, 0, sizeof(msbuf));

  int msqid;
  key_t key;

  if ((key = ftok("fifotest", 'B')) == -1) {//B唯一标识符

    perror("ftok");
    exit(1);
  }

  //使用msgget函数连接到消息队列
  if ((msqid = msgget(key, 0644 | IPC_CREAT)) == -1) {

    perror("msgget");
    exit(1);
  }
  msgrcv(msqid, &buf, sizeof(buf), 0, 0);
  puts(buf.text);

  return 0;
}
