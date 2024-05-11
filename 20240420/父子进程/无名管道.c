/***********************************************************************************
# File Name:    无名管道.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月11日 星期六 09时07分08秒
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


int main() {

  char buf[100];
  char data[100];
  int fd[2];
  pipe(fd); //生成管道:无论父子进程都有读写两端，fd[0]读描述符，fd[1]写描述符
  int fd2[2]; //实现子父通信
  pipe(fd2);
  int pid = fork();
  if (pid > 0) {
    //父进程
    close(fd[0]);
    close(fd2[1]);

    printf("父进程\n");
    strcpy(buf, "hello");
    write(fd[1], buf, sizeof(buf));
    wait(NULL);
    read(fd2[0], data, sizeof(data) - 1);
    puts(data);
    puts("---------------------");
  } else if (pid == 0) {
    //子进程
    puts("子进程");
    close(fd[1]);  //关闭写
    close(fd2[0]); //关闭第二个管道的读
    read(fd[0], buf, sizeof(buf) - 1);
    puts(buf);
    //写
    strcpy(data, "woniuxy");
    write(fd2[1], data, sizeof(data));
    puts("------------------");
  }
  return 0;
}
