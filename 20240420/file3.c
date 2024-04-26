/***********************************************************************************
# File Name:    file3.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年04月25日 星期四 16时33分07秒
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
  int fd = open("3.txt", O_WRONLY | O_APPEND | O_CREAT, 0664);
  if (fd < 0) {

    perror("open 3.txt");
    exit(0);
  }
  printf("%d\n", fd);
  char str[][30] = {"admin ad123 男 22", "zhangsan qwer123 男 13",
                    "lisi nnnn 女 88", "wangwu qwer123 男 12"};
  ssize_t count = write(fd, str[0], sizeof(str[0]));
  write(fd,"\n",1);
  write(fd, str[1], sizeof(str[0]));
  write(fd,"\n",1);
  write(fd, str[2], sizeof(str[0]));
  write(fd,"\n",1);
  write(fd, str[3], sizeof(str[0]));
  write(fd,"\n",1);
  if (count < 0) {
    perror("write fail");
  }

  return 0;
}
