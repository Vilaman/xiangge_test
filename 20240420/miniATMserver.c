/***********************************************************************************
# File Name:    miniATMserver.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年04月29日 星期一 17时38分33秒
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
#include<time.h>
#define PORT 8088

bool ischangePass=false;//修改密码
bool ismenu=false;//menu查看
bool cardisRight =false;//卡号是否成功
bool isLogin = false;//是否登录成功
int loginIndex = -1;//没有登录
bool isLoginWriteLog = false;//是否已经写入日志
typedef struct User{
char card[20];
char password[10];
}User;

char big[200];
char logs[100][150]={0};

User user[]={

	{"388888","3333"},
	{"255555","2222"},
	{"622222","1233"},
	{"123456","1222"}
};

int userlen = sizeof(user) / sizeof(user[0]);
void writeLog(char *status);
char *checkData(char *data);

int main() {

  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    perror("socket()");
    exit(1);
  }
  struct sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  server.sin_addr.s_addr = INADDR_ANY;
  if (bind(server_fd, (struct sockaddr *)&server, sizeof(server)) == -1) {

    perror("bind()");
    exit(1);
  }
  if (listen(server_fd, 1) == -1) {
    perror("listen()");
    exit(1);
  }
  int client_fd;
  struct sockaddr_in client;
  socklen_t len = sizeof(client);
  puts("等待连接中.............\n");
  if ((client_fd = accept(server_fd, (struct sockaddr *)&client, &len)) == -1) {

    perror("accept()");
    exit(1);
  }
  char *ip = inet_ntoa(client.sin_addr);
  printf("%s\n", ip);
  int readcount, writecount;
  char data[100] = "";
  char returnstr[100] = "";
  while (true) {
    memset(data, 0, sizeof(data));
    memset(returnstr, 0, sizeof(returnstr));
    readcount = read(client_fd, data, sizeof(data) - 1);
    if (readcount > 0) {
      data[readcount] = '\0';
      strcpy(returnstr, checkData(data));
    }
    if (strcmp(returnstr, "") != 0) {
		puts(returnstr);

      writecount = write(client_fd, returnstr, sizeof(returnstr));
      if (writecount > 0) {

        puts("服务器发送成功");
      }
    //判断登录成功就可以保存日志记录
	if(isLogin&&!isLoginWriteLog){
		char *status="LOGIN";
		writeLog(status);
	}else if(ischangePass&&!isLoginWriteLog){

		char *status="Changepassword";
		writeLog(status);
	}
  }
  }
  return 0;
}



char *checkData(char *data) {
  puts(data);
  if (!cardisRight) {
    puts(data);
    for (int i = 0; i < userlen; i++) {
      if (strcmp(data, user[i].card) == 0) {

        loginIndex = i;
        cardisRight = true;
		isLogin=false;
        return "cardRight";
      }
    }
  } else if (!isLogin) {
    if (strcmp(data, user[loginIndex].password) == 0) {
      puts(user[loginIndex].password);
      isLogin = true;
	  ismenu=false;
      return "loginsuccess";
    }
  } else if (!ismenu) {

	if(strcmp(data,"showcard")==0){
		printf("%s %s",user[loginIndex].card,user[loginIndex].password);
		sprintf(big,"%s %s",user[loginIndex].card,user[loginIndex].password);
		return big;
	}else if(!ischangePass){

		printf("此时发送的值是%s\n",data);
		strcpy(user[loginIndex].password,data);
		ischangePass=true;
		return "changesuccess";

	}else if(strcmp(data,"exit")==0){

		puts("退出菜单");
		ismenu=true;
		cardisRight=false;
		ischangePass=false;
		return "exit";
	}else{

		puts("incorrect input");
	}

  }

  return "输入错误";
}

void writeLog(char *status){
	time_t t=time(NULL);
	struct tm *now=localtime(&t);
	char timestr[40];
	strftime(timestr,sizeof(timestr)-1,"%y年%m月%d日 %H:%M:%S\n",now);
	timestr[strlen(timestr)]='\0';

	FILE *fp=fopen("log.txt","a+");
	if(fp==NULL){

		perror("fopen()");
		exit(1);
	}
	fprintf(fp,"%s %s\n",status,timestr);
	puts("写入日志记录");
	fclose(fp);
}


