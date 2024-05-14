/***********************************************************************************
# File Name:    woniukeeper3.c
# Author:       WENPINGXIANG
# Mail:         630556320@qq.com
# Created Time: 2024年04月18日 星期四 03时57分52秒
 **********************************************************************************/

#include <stdio.h>
#include<stdbool.h> 
#include<string.h> 
#include<stdlib.h>
//设置用户结构体变量
typedef struct Emp{
	char username[16];
	char password[16];
	char sex[5];
	int age;
}emp;//设置别名emp

void menuone();
void menutwo();
int login();
void regi();
bool isExsit();
bool userIsexsit(char *name);
void showAll();
char delete ();
char changeInfo(int logindex);
char changePassword(int logindex);

emp emprog[10];

int len=sizeof(emprog)/sizeof(emprog[0]);
int logindex=-1;//定义登陆者下标

int main() {

  menuone();

  return 0;
}


void menuone() {
  bool isrun = true;
  printf("---------woniu keeper-----------\n");
  do {
    printf("1.登录系统\n");
    printf("2.注册新用户\n");
    printf("3.退出系统\n");
    int opt;
    scanf(" %d", &opt);
    switch (opt) {
    case 1:
      if (isExsit()) {
        login();
      } else {
        printf("当前没有用户请您先注册\n");
      }
      break;
    case 2:
      regi();
      break;
    case 3:
      isrun = false;
      break;
    default:
      printf("请输入正确的选项\n");
      break;
    }

  } while (isrun);
}

void menutwo() {
  bool isrun = true;
  printf("----------woniu keeper二级菜单-----------\n");
  do {
    printf("1.刷卡\n");
    printf("2.查询所有用户\n");
    printf("3.修改个人密码\n");
    printf("4.修改个人信息\n");
    printf("5.删除用户\n");
    printf("6.返回上一级\n");
    int act;
    scanf(" %d", &act);
    switch (act) {
    case 1:
      printf("请刷卡\n");
      break;
    case 2:
      showAll();
      break;
    case 3:
      changePassword(logindex);
      break;
    case 4:
      changeInfo(logindex);
      break;
    case 5:
      delete ();
      break;
    case 6:
      isrun = 0;
    }
  } while (isrun);
}

//登录模块
int login(){
	bool Islog=false;
	char iusername[16];
	char ipasswd[16];
	printf("请输入您的用户名\n");
	scanf(" %15s",iusername);
	printf("请输入您的密码\n");
	scanf(" %15s",ipasswd);
	for(int i=0;i<len;i++){
		if(strcmp(iusername,emprog[i].username)==0 && strcmp(ipasswd,emprog[i].password)==0){
			Islog=true;
			logindex=i;
			break;
		}
	}

if (Islog) {
  printf("登陆成功\n");
  menutwo();
} else {
  printf("登录失败，回到上级菜单\n");
	return -1;
}
}
//注册模块
void regi() {
  char name[16];
  printf("请输入您要注册的用户名\n");
  scanf(" %s", name);
  //判断用户名是否已经存在
  bool Userisnotexist = userIsexsit(name);
  if (!Userisnotexist) {

    printf("您输入的用户名已存在注册失败\n");
    return;
  }

  for (int i = 0; i < len; i++) {
    if (strcmp(emprog[i].username, "") == 0) {
      strcpy(emprog[i].username, name);
      printf("请输入您的密码\n");
      scanf(" %15s", emprog[i].password);
      printf("请输入您的年龄\n");
      scanf(" %d", &emprog[i].age);
      printf("请输入您的性别\n");
      scanf(" %4s", emprog[i].sex);
      printf("用户%s.%s,%d,%s,注册成功\n", emprog[i].username,
             emprog[i].password, emprog[i].age, emprog[i].sex);
      return;
    }
  }
  printf("注册人数已满\n");
}

bool isExsit() {
  //判断登录前有没有用户名存在
  bool isnotExistuser = false;
  for (int i = 0; i < len; i++) {
    if (strcmp(emprog[i].username, "") != 0) {
      isnotExistuser = true;
    }
  }
  return isnotExistuser;
}

//判断当前输入用户名是否已经存在返回false或true
bool userIsexsit(char *name) {
  bool userisexist = true;
  for (int i = 0; i < len; i++) {
    if (strcmp(emprog[i].username, name) == 0) {
      userisexist = false;
    }
  }
  return userisexist;
}

//查看所有用户函数
void showAll() {
  for (int i = 0; i < len; i++) {
    if (strcmp(emprog[i].username, "") != 0) {
      printf("序号%d,用户名：%s,密码：%s,年龄：%d,性别：%s\n", i,
             emprog[i].username, emprog[i].password, emprog[i].age,
             emprog[i].sex);
    }
  }
}

//删除用户函数
char delete () {
  bool isdel = false;
  int delindex = -1; //定义一个下标变量
  char Delusername[16];
  printf("请输入您要删除的用户名\n");
  scanf(" %15s", Delusername);
  for (int i = 0; i < len; i++) {
    if (strcmp(emprog[i].username, Delusername) == 0) {
      delindex = i;
      isdel = true;
    }
  }
  if (isdel) {
    strcpy(emprog[delindex].username, "");
    strcpy(emprog[delindex].password, "");
    strcpy(emprog[delindex].sex, "");
	emprog[delindex].age = 0;
    printf("删除成功\n");
  } else {
    printf("用户名不存在\n");
  }
}

//修改用户信息
char changeInfo(int logindex) {
  printf("请输入您的用户名\n");
  scanf(" %s", emprog[logindex].username);
  printf("请输入您的年龄\n");
  scanf(" %d", &emprog[logindex].age);
  printf("请输入您要修改的性别\n");
  scanf(" %s", emprog[logindex].sex);
  printf("您的信息已修改为%s %s %d\n", emprog[logindex].username, emprog[logindex].sex,
         emprog[logindex].age);
  return 0;
}

//修改密码
char changePassword(int logindex) {
  char inputpasswd[16];
  printf("请输入您的旧密码\n");
  scanf(" %15s", inputpasswd);
  if (strcmp(inputpasswd,emprog[logindex].password) == 0) {
    printf("请输入你的新密码\n");
    scanf(" %15s",emprog[logindex].password);
    printf("密码修改成功\n");
  } else {
    printf("密码不对修改失败\n");
  }
  return 0;
}

