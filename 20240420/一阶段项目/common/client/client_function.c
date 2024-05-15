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


Info logininfo;

//连接服务器后选择功能菜单 optype为当前操作类型
void mainmenu(bool isRun, int client_fd) {

  puts("-----------------------------");
  puts("---------1.登录系统----------");
  puts("---------2.退出系统----------");
  puts("-----------------------------");

  char dataWithback[41];
  char chose;
  scanf(" %c", &chose);

  while (getchar() != '\n');
  // 清理输入缓冲区，移除多余的字符，包括换行符
  switch (chose) {
  case '1':
	RequestInfo(CLIENT_LOGIN,client_fd);//调用发送操作函数login信息给服务器
    if (Returnquest(LOGIN_ALLOW, client_fd)) {
      if(strcmp(userLogin(client_fd),ADMIN_LOGIN)==0){
			adminMenu(client_fd);//调用管理员菜单				  
	  }else if(strcmp(userLogin(client_fd),NORMALUSER_LOGIN)==0){
			normalUserMenu(client_fd);//调用普通用户菜单
	  }else{  }
    }
    break;
  case '2':
    isRun = false;
    break;
  default:
    puts("请输入正确选项");
    break;
  }
}



//发送操作信息
void RequestInfo(char *optype, int client_fd) {
  int writecount;
  if (strcmp(optype, "") != 0) {
    Backerror_Client(REQUEST_ERROR);
    return;
  } else {
    if ((writecount = write(client_fd, optype, client_fd)) < 0) {
      perror("Requestinfo");
      return;
    }
  }
  return;
}

//服务器返回是否接受操作函数
bool Returnquest(char *reoptype,int client_fd){
	char comeBacktype[40];
	int readcount;
	if((readcount=read(client_fd,comeBacktype,sizeof(comeBacktype)-1))>0){
		comeBacktype[readcount]='\0';
		if(strcmp(comeBacktype,reoptype)==0){
			return true;
		}
	}
	return false;
}



//登录函数
char* userLogin(int client_fd) {
  char input[60];
  int logincount=0;
  while(logincount<=3){
	logincount++;
	memset(input,0,sizeof(input));
    printf("请输入你的用户名\n");
    scanf(" %19s", logininfo.username);
    printf("请输入你的密码\n");
    scanf(" %10s", logininfo.password);
  snprintf(input, sizeof(input) - 1, "%s %s", logininfo.username, logininfo.password);
  if (strcmp(input, "") == 0) {           //如果输入的为空
    Backerror_Client(LOGIN_FORMAT_ERROR); //调用格式错误函数
    continue;
  }
  int writecount;
  if ((writecount = write(client_fd, input, sizeof(input))) < 0) {
    perror("write()");
    exit(-1);
  }
  puts("正在登录请稍等");
  sleep(1);
  //判断服务器返回信息
  char reData[40];
  reDatawithServer(reData, client_fd);
  if (strcmp(reData, "LOGIN_USERNAME_ERROR") == 0) {
	  Backerror_Client(LOGIN_USERNAME_ERROR);
  }else if(strcmp(reData,"LOGIN_PASSWORD_ERROR")==0){
		Backerror_Client(LOGIN_PASSWORD_ERROR);
  }else if(strcmp(reData,"LOGIN_THREE_TIMES_ERROR")==0){
		Backerror_Client(LOGIN_THREE_TIMES_ERROR);
		return "LOGIN_THREE_TIMES_ERROR";
  }else if(strcmp(reData,ADMIN_LOGIN)==0){
		return ADMIN_LOGIN;
  }else if(strcmp(reData,NORMALUSER_LOGIN)==0){
		return NORMALUSER_LOGIN;
  }
  else{
		Backerror_Client(SERVER_COMBACK_ERROR);
		return "SERVER_COMBACK_ERRORL";
  }
  }
  return NULL;
}

//判断服务端返回信息（登录模块）
void reDatawithServer(char *reData,int client_fd){
	int loginReadcount;
	if((loginReadcount=read(client_fd,reData,sizeof(reData)-1))>0){
		reData[loginReadcount]='\0';
		return;
	}
	Backerror_Client(SERVER_COMBACK_ERROR);
	return;
}



//判断是否是管理员登录
bool isAdmin_login(char *comeBackData){

	if(strcmp(comeBackData,ADMIN_LOGIN)==0){
		return true;
	}
	return false;
}

//管理员登录菜单
void adminMenu(int client_fd) {
  bool isadminlogin = true;
  char input[N];
  while (isadminlogin) {
    puts("---------------------------");
    puts("------1.注册新用户---------");
    puts("------2.删除用户-----------");
    puts("------3.修改用户信息-------");
    puts("------4.查询用户信息-------");
    puts("------5.查看用户操作记录---");
    puts("------6.退出登录-----------");
    puts("---------------------------");
    char chose_fun;
    scanf(" %c", &chose_fun);
    while (getchar() != '\n');
    switch (chose_fun) {
    case '1':
      RequestInfo(USER_REGISTER,client_fd);
      if (Returnquest(REGISTER_ALLOW,client_fd)) {
        _Register(client_fd);
      }
      break;
    case '2':
      RequestInfo(DELETE_USER,client_fd);
      if (Returnquest(DELETE_ALLOW,client_fd)) {
        Delete(client_fd);
      }
      break;
    case '3':
      RequestInfo(CHANGE_INFO,client_fd);
      if (Returnquest(CHANGE_ALLOW,client_fd)) {
        ChangeInfo(CHANGE_INFO,client_fd);
      }
      break;
    case '4':
      RequestInfo(QUERY_INFO,client_fd);
      if (Returnquest(QUERYINFO_ALLOW,client_fd)) {
        Queryinfo(QUERY_INFO,client_fd);
      }
      break;
    case '5':
      RequestInfo(QUERY_OPRATION,client_fd);
      if (Returnquest(QUERYOP_ALLOW,client_fd)) {
        QueryOpration(QUERY_OPRATION,client_fd);
      }
      break;
    case '6':
      isadminlogin = false;
      break;
    default:
      Backerror_Client(SELECT_OPERATION_ERROR);
      break;
    }
  }
}

//注册新用户

void _Register(int client_fd) {
  User u = {0}; //定义一个user类型结构体变量
  char input[N];
  puts("请输入新账户");
  scanf(" %s", u.account); //不能是admin

  puts("请输入名字");
  scanf(" %s", u.name);
  //密码需满足条件
  puts("请输入密码");
  scanf(" %s", u.pass);

  snprintf(input, sizeof(input) - 1, "%s %s %s ", u.account, u.name, u.pass);
  int writecount;
  if((writecount=write(client_fd,input,sizeof(input)))<0){
		perror("Register()");
		return;
  }
	int readcount;
	char recvData[30];
  if ((readcount = read(client_fd, recvData, sizeof(recvData) - 1)) > 0) {
    recvData[readcount] = '\0';
    if (strcmp(recvData, REGISTER_SUCCESS) == 0) {
      puts(recvData);
    } else if (strcmp(recvData, "REGISTER_EXIST_ERROR") == 0) {
      Backerror_Client(REGISTER_EXIST_ERROR);
    } else if (strcmp(recvData, "REGISTER_OTHER_ERROR") == 0) {
      Backerror_Client(REGISTER_OTHER_ERROR);
    }
  } else {
    Backerror_Client(SERVER_COMBACK_ERROR);
  }
}



//删除用户信息
void Delete(int client_fd) {
  char Account[30]; //定义变量
  puts("请输入你要删除的账户");
  scanf(" %s", Account);
  //判断是否是admin
  int writecount;
  if ((writecount = write(client_fd, Account, sizeof(Account))) < 0) {
    perror("Delete()");
    return;
  }

  int readcount;
  char recvData[30];
  if ((readcount = read(client_fd, recvData, sizeof(recvData) - 1)) > 0) {
    recvData[readcount] = '\0';
    if (strcmp(recvData, DELETE_SUCCESS) == 0) {
      puts(recvData);
    } else if (strcmp(recvData, "DELETE_NOTEXIST_ERROR") == 0) {
      Backerror_Client(DELETE_NOTEXIST_ERROR);
    } else {

      Backerror_Client(DELETE_OTHER_ERROR);
    }
  } else {
    Backerror_Client(SERVER_COMBACK_ERROR);
  }
}



//修改用户信息
void ChangeInfo(char *usertype, int client_fd) {
  User u = {0}; //定义一个user类型结构体变量
  char input[N];
  if (strcmp(usertype, CHANGE_INFO) == 0) {
    puts("请输入要修改的账户");
    scanf(" %s", u.account); //不能是admin

  } else {
    strcpy(u.account, logininfo.username);
  }
  puts("请输入要修改的名字");
  scanf(" %s", u.name);
  //密码需满足条件
  puts("请输入要修改的密码");
  scanf(" %s", u.pass);

  puts("请输入要修改的性别");
  scanf(" %s", u.sex);
  puts("请输入要修改的年龄");
  scanf(" %d", &u.age);
  puts("请输入要修改的地址");
  scanf(" %s", u.address);
  snprintf(input, sizeof(input) - 1, "%s %s %s %s %d %s", u.account, u.name,
           u.pass, u.sex, u.age, u.address);
  int writecount;
  if ((writecount = write(client_fd, input, sizeof(input))) < 0) {
    perror("Changeinfo");
    return;
  }
  int readcount;
  char recvData[30];
  if ((readcount = read(client_fd, recvData, sizeof(recvData) - 1)) > 0) {
    recvData[readcount] = '\0';
    if (strcmp(recvData, CHANGE_SUCCESS) == 0) {
      puts(recvData);
    } else if (strcmp(recvData, "CHANGE_NOTEXIST_ERROR") == 0) {
      Backerror_Client(CHANGE_NOTEXIST_ERROR);
    } else if (strcmp(recvData, "CHANGE_OTHER_ERROR") == 0) {
      Backerror_Client(CHANGE_OTHER_ERROR);
    }
  } else {
    Backerror_Client(SERVER_COMBACK_ERROR);
  }
  return;
}



//查询用户信息
void Queryinfo(char *usertype,int client_fd){
	int writecount;
	if((writecount=write(client_fd,usertype,sizeof(usertype)))<0){
			perror("QueryOpration");
			return;
	}
	puts("正在查询日志");
	int readcount;
	char comebackdata[MAXSIZE];
	if((readcount=read(client_fd,comebackdata,sizeof(comebackdata)-1))>0){
		comebackdata[readcount]='\0';
		puts(comebackdata);
	}else{
		Backerror_Client(QUERYINFO_FAIL);
	}
	return;
}
	


//查看操作日志
void QueryOpration(char *usertype,int client_fd){
	int writecount;
	if(writecount=write(client_fd,usertype,sizeof(usertype))<0){
			perror("QueryOpration");
			return;
	}
	puts("正在查询日志");
	int readcount;
	char comebackdata[MAXSIZE];
	if((readcount=read(client_fd,comebackdata,sizeof(comebackdata)-1))>0){
		comebackdata[readcount]='\0';
		puts(comebackdata);
	}else{
		Backerror_Client(QUERYOP_FAIL);
	}
	return;
}


//普通用户操作菜单函数
void normalUserMenu(int client_fd){
  bool isnormallogin = true;
  char input[N];
  while (isnormallogin) {
    puts("---------------------------");
    puts("------1.修改用户信息-------");
    puts("------2.查询用户信息-------");
    puts("------3.查看操作记录-------");
    puts("------4.退出登录-----------");
    puts("---------------------------");
    char chose_fun;
	char loginuser[21];
	strcpy(loginuser,logininfo.username);
    scanf(" %c", &chose_fun);
    while (getchar() != '\n');
    switch (chose_fun) {
    case '1':
      RequestInfo(NORMAL_CHANGEINFO,client_fd);
      if (Returnquest(NORMALCH_ALLOW,client_fd)) {
        ChangeInfo(loginuser,client_fd);
      }
	  break;
    case '2':
      RequestInfo(NORMAL_QUERYINFO,client_fd);
      if (Returnquest(NORMALQI_ALLOW,client_fd)) {
        Queryinfo(loginuser,client_fd);
      }
      break;
    case '3':
      RequestInfo(NORMAL_QUERYOP,client_fd);
      if (Returnquest(NORMALQOP_ALLOW,client_fd)) {
        QueryOpration(loginuser,client_fd);
      }
      break;
    case '4':
      isnormallogin = false;
      break;
    default:
      Backerror_Client(SELECT_OPERATION_ERROR);
      break;
	}

}

}



























