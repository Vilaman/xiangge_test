/***********************************************************************************
# File Name:    miniATM客户端.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月01日 星期三 16时09分57秒
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
#define PORT 8088
#define N 200

char input[N];
char resultdata[N];
bool islogin = false;
bool ispassword = false;
bool isRun=true;
bool ismenu=false;
void socketconnect(char *input);
void inputfun();
void menu();


int main(){
	socketconnect(input);
    return 0;
}


void socketconnect(char *input){
int client_fd=socket(AF_INET,SOCK_STREAM,0);
if(client_fd<0){

	perror("socket()");
	exit(1);
}
struct sockaddr_in server;
server.sin_family=AF_INET;
server.sin_port=htons(PORT);
server.sin_addr.s_addr=inet_addr("127.0.0.1");

if(connect(client_fd,(struct sockaddr *)&server,sizeof(server))==-1){

	perror("connect()");
	exit(1);
}
puts("链接服务器成功");


int readcount,writecount;
while(isRun){
inputfun();
if(strcmp(input,"")!=0){
	writecount=write(client_fd,input,sizeof(input));
	if(writecount>0){

		puts("发送成功");
	}else{

		puts("发送失败");
		exit(1);
	}
}
readcount=read(client_fd,resultdata,sizeof(resultdata)-1);
//printf("返回的值是%s\n",resultdata);
if(readcount>0){
	resultdata[readcount]='\0';

	if(strcmp(resultdata,"cardRight")==0){

		puts("卡号输入正确");
		ispassword=true;
	}
	else if(strcmp(resultdata,"loginsuccess")==0){

		puts("登录成功");
		islogin=true;
		ismenu=false;
	}
	else if(strcmp(resultdata,"")!=0){
		
		printf("%s\n",resultdata);
	}else if(strcmp(resultdata,"changesuccess")==0){

		printf("密码修改成功\n");
	}else{


	}
}
}
}

//拿到input的值
void inputfun() {
  memset(input, 0, sizeof(input));
  if (!ispassword) {
    puts("请输入你的卡号");
    scanf(" %s", input);
  } else if (!islogin) {

    puts("请输入你的密码");
    scanf(" %s", input);
  }else if(!ismenu){

	menu();

  }
}

//二级菜单
void menu(){
printf("-----二级菜单-------\n");
printf("1.查看银行卡 2.修改密码 3.退出\n");
char chose;
scanf(" %c",&chose);
switch(chose){
	case '1':
		strcpy(input,"showcard");
		break;
	case '2':
		puts("请输入你的新密码");
		scanf(" %s",input);
		break;
	case '3':
		ismenu=true;
		islogin=false;
		ispassword=false;
		strcpy(input,"exit");
		break;
	default:
		puts("请输入正确选项");
		strcpy(input,"");
		break;
	}
	return;
}


