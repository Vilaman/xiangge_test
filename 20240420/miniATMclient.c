/***********************************************************************************
# File Name:    miniATMclient.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年04月29日 星期一 17时37分59秒
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
#define PORT 8080

int main() {

  int client_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (client_fd < 0) {
    perror("socket()");
    exit(1);
  }
  struct sockaddr_in server;

  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  server.sin_addr.s_addr = inet_addr("127.0.0.1");

  if (connect(client_fd, (struct sockaddr *)&server, sizeof(server)) == -1) {

    perror("connect()");
    exit(1);
  }
  puts("连接服务器成功");

  bool isuser = 0;
  bool ispass = false;
  char input[50];
  char resultdata[30];
  do {
    memset(input, 0, sizeof(input));
    memset(resultdata, 0, sizeof(resultdata));

    if (!isuser) {
      memset(input, 0, sizeof(input));
      puts("请输入你的卡号\n");
      scanf(" %s", input);
    } else if (!ispass) {
      memset(input, 0, sizeof(input));
      puts("请输入你的密码\n");
      scanf(" %s", input);
    }
    if (strcmp(input, "") != 0) {
      int writecount = write(client_fd, input, sizeof(input));
      if (writecount > 0) {
        puts("正在查询");
      }
    }

    int readcount = read(client_fd, resultdata, sizeof(resultdata) - 1);
	printf("%d\n",readcount);
    if (readcount > 0) {
		resultdata[readcount]='\0';
      printf("接收到的%s", resultdata);
      if (strcmp(resultdata, "cardRight") == 0) {
        printf("%s", resultdata);
        puts("卡号输入正确");
        isuser = true;
      } else if (strcmp(resultdata, "loginsuccess") == 0) {
        printf("登陆成功\n");
        isuser = true;
      } else {
        puts("请重新输入");
      }
    }

  } while (true);

  return 0;
}
