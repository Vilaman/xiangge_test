/***********************************************************************************
# File Name:    file1.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年04月25日 星期四 14时35分05秒
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

  int fd = open("./1.txt", O_CREAT | O_RDONLY);

  if (fd == -1) {
    perror("open fail");
    return 0;
  }
  printf("fd:%d\n", fd);
  char buf[10] = "";
  ssize_t count;
  int len = sizeof(buf);
  while ((count = read(fd, buf, len - 1) > 0)) {
    buf[len - 1] = '\0';
    printf("%s", buf);
    strcpy(buf, "");
  }

  return 0;
}
