/***********************************************************************************
# File Name:    管道.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月11日 星期六 11时32分40秒
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
#include <sys/stat.h>
#include <errno.h>


char path[] = "/tmp/my_fifo2";
int main() {
    // 创建有名管道。第一个参数是路径名，第二个参数是文件权限设置。
    // 注意如果文件已经存在，mkfifo 不会做任何事情，但是会返回一个错
    if (mkfifo(path, 0666) == -1 && errno != EEXIST) {
      perror("mkfifo error");
      exit(1);
    }
	puts("2344");
    int pf = open(path, O_WRONLY);
    if (pf == -1) {
      perror("open error");
      exit(1);
    }
    //写入数据到管道
	char *input="hello,1";
	write(pf,input,sizeof(input));
    close(pf);
    return 0;
  }
