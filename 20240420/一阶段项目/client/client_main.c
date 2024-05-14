/***********************************************************************************
# File Name:    client_main.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月14日 星期二 16时09分32秒
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
#include "client_error.h"
#include "client_function.h"
#include "client_info.h"


User u;//员工信息结构体变量


int main() {
  //网络套接字连接服务器
  int client_fd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  server.sin_addr.s_addr = inet_add("127.0.0.1");

  if (connect(client_fd, (struct sockaddr *)&server, sizeof(server)) < 0) {

    perror("CONNECT()");
    exit(-1);
  }

  puts("连接服务器成功");
  bool isloginpass = false; //是否成功登录
  bool isRun = true;        //定义外层循环
  bool isMainmenu = true;   //定义选择菜单执行
  char input[N]; //定义发送字符串
  char comeBackData[MAXSIZE];
  while (isRun) {
    memset(u, 0, sizeof(u));         //清理u结构体
    memset(input, 0, sizeof(input)); //清理input
    if (isMainmenu) {
      mainmenu(bool isRun);//调用外层菜单
    } else if (isAdmin_login(comeBackData)) {
		adminMenu();
    }else if(!isAdmin_login(comeBackData)){

		normalUserMenu();
	}
  }

  return 0;
}
