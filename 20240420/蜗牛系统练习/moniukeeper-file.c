/***********************************************************************************
# File Name:    moniukeeper-file.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年04月25日 星期四 19时28分34秒
 **********************************************************************************/

#include <stdio.h>
#include<string.h> 
#include<stdbool.h>
#include<stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>


typedef struct User{
char *username;
char *password;
char *sex;
char *age;
}User;

User u={NULL,NULL,NULL,NULL};

void readuser();
char writeuser(char *big);
void splitStr(char *comb);
char *switchtoBig(User u);
void menu();
void login();
void reg();



int main() {
	readuser();
	menu();	
	return 0; 
}


void readuser() {
  int fd = open("message.txt", O_RDONLY | O_CREAT,0664);
  if (fd < 0) {
    perror("open file");
    exit(0);
  }
  char buf[1024] = "";
  ssize_t count = read(fd, buf, 1023);
  if (count < 0) {
    perror("read()");
    exit(0);
  }
  close(fd);
  splitStr(buf);
}



char writeuser(char *big) {
  int fd = open("message.txt",O_WRONLY | O_CREAT | O_APPEND, 0664);
  if (fd < 0) {

    perror("open()");
    exit(0);
  }
  int size = sizeof(u);

  ssize_t count = write(fd, big, size);
  if(count<0){
	perror("write()");
	exit(0);
  }
  close(fd);
}



void splitStr(char *comb) {

  char *ch = " ";
  char *child = NULL;
  int i = 0;
  do {
    if (child == NULL) {
      child = strtok(comb, ch);
      u.username = child;
    } else {
      child = strtok(NULL, ch);
      if (i == 0) {
        u.password = child;
      } else if (i == 1) {
        u.sex = child;
      } else if (i == 2) {
        u.age = child;
      }
      i++;
    }
  } while (child != NULL);
}


char *switchtoBig(User u) {
  char *big = NULL;
  char *str[] = {u.username, u.password, u.sex, u.age};
  int len = sizeof(str) / sizeof(str[0]);
  printf("请输入你要注册的用户名22%s\n",u.username);
  for (int i = 0; i < len; i++) {
  printf("请输入你要注册的用户名33\n");
    strcat(big, str[i]);
  printf("请输入你要注册的用户名44\n");
    if (i != len - 1) {
  printf("请输入你要注册的用户名\n");
      strcat(big, " ");
  printf("请输入你要注册的用户名\n");
    }
  }
  return big;
}



void menu() {
  printf("欢迎来到门禁系统\n");
    bool isrun = true;
  do {
    printf("-------------------------------------------\n");
    printf("1.登录系统    2.注册系统     3.退出\n");
    printf("-------------------------------------------\n");
    char opt;
    scanf(" %c", &opt);
    switch (opt) {

    case '1':
      login();
      break;
    case '2':
      reg();
      break;
    case '3':
      isrun = false;
      break;
    default:
      printf("请输入正确的选项\n");
      break;
    }
  }while(isrun);
}


void login(){
	if(u.username==NULL){

		printf("请先注册\n");
		return;
	}
	char name[16];
	char pass[16];
	printf("请输入你的用户名\n");
	scanf(" %s",name);
	printf("请输入你的密码\n");
	scanf(" %s",pass);
	if(strcmp(u.username,name)==0 && strcmp(u.password,pass)==0){
		printf("登录成功\n");
	}
	printf("登录失败\n");
	return;
	
}


void reg() {
  char newname[16];
  char newpass[16];
  char newsex[16];
  char newage[2];
  printf("请输入你要注册的用户名\n");
  scanf(" %s", newname);
  if (strcmp(newname, u.username) == 0) {
    printf("注册失败\n");
    return;
  }
  strcpy(u.username,newname);
  printf("请输入密码\n");
  scanf(" %s", newpass);
  u.password=newpass;
  printf("请输入性别\n");
  scanf(" %s", newsex);
  u.sex=newsex;
  printf("请输入年龄\n");
  scanf(" %s",newage);
  u.age=newage;
  char *big=switchtoBig(u);
  printf("hbjnkml");
  writeuser(big);
  printf("注册成功\n");
  return;
}
