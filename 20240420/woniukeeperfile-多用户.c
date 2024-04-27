/***********************************************************************************
# File Name:    woniukeeperfile-多用户.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年04月26日 星期五 19时26分25秒
 **********************************************************************************/

#include <stdio.h>
#include<string.h> 
#include<stdbool.h>
#include<stdlib.h>
#include"mySLNode.h"
#define N 50

int line=0;//定义文件是否存在用户行数
Node *user=NULL; //定义单链表指针

void menuone();
void menutwo();
bool isExistuser();
bool isExistinTXT(char *newname, int line);
void login();
int UserReadandInit();
void UserWrite(User u);
void regIster();





int main() {
  showNode(user);
  UserReadandInit();
  menuone();
  showNode(user);
  Nodefree(&user);

  return 0;
}


void menuone() {

  bool isRun = true;
  printf("欢迎来到蜗牛门禁系统\n");
  do {
    printf("-----------------------------------------\n");
    printf("1.登录系统   2.注册系统     3.退出系统\n");
    printf("-----------------------------------------\n");
	char opt;
	scanf(" %c",&opt);
	switch(opt){
	case '1':
		login();
		break;
	case '2':
		regIster();
		break;
	case '3':
		isRun=false;
		break;
	default:
		printf("请输入正确的选项\n");
		break;
	}
  }while(isRun);
  return;

}


void menutwo() {
  bool isRuntwo = true;
  printf("欢迎来到二级菜单\n");
  do {
    printf("-----------------------------------------\n");
    printf("1.查看信息   2.修改密码     3.修改信息\n");
    printf("4.删除用户   5.退出系统\n");
    printf("-----------------------------------------\n");
    char seLe;
    scanf(" %c", &seLe);
    switch (seLe) {

    case '1':
//      showData();
      break;
    case '2':
  //    ChangePass();
      break;
    case '3':
   //   ChangeData();
      break;
    case '4':
  //    DelUser();
      break;
    case '5':
      isRuntwo = false;
      break;
    default:
      printf("请输入正确的选项\n");
      break;
    }

  } while (isRuntwo);
}

//通过文件行数来判断当前是否有用户存在
bool isExistuser() {

  if (line == 0) {
    return true;
  }
  return false;
}



bool isExistinTXT(char *newname, int line) {
	User u;
  FILE *fp1 = fopen("message.txt", "r");
  if (fp1 == NULL) {
    perror("open file");
    exit(1);
  }
  for (int i = 0; i < line; i++) {
    fscanf(fp1, "%s %s %s %d\n", u.username, u.password, u.sex, &u.age);
    if (strcmp(u.username, newname) == 0) {
      return true;
    }
  }
  fclose(fp1);
  fp1 = NULL;
  return false;
}




void login() {
  char loginname[20];
  char loginpass[20];
  printf("line:%d\n",line);
  if (isExistuser()) {
    printf("当前没有用户存在请先注册\n");
    return;
  }
  printf("请输入用户名\n");
  scanf(" %s", loginname);
  printf("请输入密码\n");
  scanf(" %s", loginpass);
  int logind = isDataexist(user, loginname, loginpass);
  if (logind == -1) {

    printf("登录失败\n");
    return;
  }
  printf("登录成功\n");
}




//读取message.txt里面的用户信息并初始化链表
int UserReadandInit() {
	User *u;

  FILE *readpf = fopen("message.txt", "r");
  if (readpf == NULL) {

    perror("fopen()");
    exit(1);
  }
  char buf[N];
  while (fgets(buf, N, readpf) != NULL) {
    if (buf[strlen(buf) - 1] == '\n')
      line++;
  }
  printf("Total line : % d\n", line);
  if (line == 0) {

    printf("当前没有用户请先注册\n");
    return 0;
  }
 // for (int i = 0; i < count; i++) {
    fscanf(readpf, "%s %s %s %d\n", u->username, u->password, u->sex, &u->age);
    printf(" 初始化：%s %s %s %d\n",u->username, u->password, u->sex, u->age);
    appendNode(&user, u);//初始化链表
//  }

  fclose(readpf);
  readpf=NULL;
  return line;
}


//写入到message.txt中
void UserWrite(User u) {
  FILE *writefp = fopen("message.txt", "a");
  if (writefp == NULL) {
    perror("userwrite open file");
    exit(1);
  }
  fprintf(writefp, "%s %s %s %d\n", u.username, u.password, u.sex, u.age);
  fclose(writefp);
  writefp = NULL;
}


//注册模块
void regIster(){
	char regname[20];
	printf("请输入用户名\n");
	scanf(" %s",regname);
	if(isExistinTXT(regname,line)){
	printf("用户名已存在注册失败\n");
	return;
	}
	printf("验证\n");
	User u1;
	strcpy(u1.username,regname);
	printf("请输入密码\n");
	scanf(" %s",u1.password);
	printf("请输入性别·\n");
	scanf(" %s",u1.sex);
	printf("请输入年龄\n");
	scanf(" %d",&u1.age);
    printf("%s %s %s %d\n",u1.username, u1.password, u1.sex, u1.age);
	appendNode(&user,&u1);
	showNode(user);
	UserWrite(u1);
	line++;
	printf("注册成功\n");
	return;
}



void showData(){




}


void ChangePass(){



}



void ChageData(){



}


void DelUser(){



}


