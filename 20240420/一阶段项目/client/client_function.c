/***********************************************************************************
# File Name:    client_function.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月14日 星期二 16时09分54秒
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
#include "client_function.h"
#include "client_info.h"
#include "client_error.h"

//连接服务器后选择功能菜单
void mainmenu(bool isRun) {
  puts("-----------------------------");
  puts("---------1.登录系统----------");
  puts("---------2.退出系统----------");
  puts("-----------------------------");
  char chose;
  scanf(" %c", &chose);
  while (getchar() != '\n');
// 清理输入缓冲区，移除多余的字符，包括换行符
  switch (chose) {
  case '1':
    userLogin(char *input);
    break;
  case '2':
    isRun = false;
    break;
  default:
    puts("请输入正确选项");
    break;
  }
}

//登录函数
void userLogin(char *input,int client_fd){
	char username[20];
	char password[11];
	char comb[40];
	printf("请输入你的用户名\n");
	scanf(" %19s",username);
	printf("请输入你的密码\n");
	scanf(" %10s",password);
	snprintf(comb,sizeof(comb)-1,"%s %s",username,password);
	strcpy(input,comb);
	int writecount;
	if((writecount=write(client_fd,input,sizeof(input)))<0){
		perror("write()");
		exit(-1);
	}
}



















