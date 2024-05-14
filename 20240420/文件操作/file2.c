/***********************************************************************************
# File Name:    file2.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年04月25日 星期四 15时48分24秒
 **********************************************************************************/

#include <stdio.h>
#include<string.h> 
#include<stdbool.h>
#include<stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
int main() {
  int fd1 = open("1.txt", O_RDONLY);
  if (fd1 < 0) {
    perror("open 1.txt");
    exit(0);
  }
  int fd2 = open("2.txt", O_WRONLY | O_CREAT, 0664);
  if (fd2 < 0) {
    perror("open 2.txt");
    exit(0);
  }
  char buf[20] = "";
  ssize_t readcount, writecount;
  while ((readcount =read (fd1, buf, sizeof(buf) - 1)) > 0) {
    buf[readcount] = '\0';
    printf("\n读取内容：%s", buf);
    writecount = write(fd2, buf, readcount);
    if (writecount < 0) {
      perror("write()");
      exit(0);
    }
  }


    return 0;
}
