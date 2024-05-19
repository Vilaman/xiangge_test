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
#include <ctype.h>
#include "client_function.h"
#include "client_info.h"
#include "client_error.h"


Info logininfo;

//连接服务器后选择功能菜单 optype为当前操作类型
void mainmenu(bool isRun, int client_fd) {
  do {
    puts("-----------------------------");
    puts("---------1.登录系统----------");
    puts("---------2.退出系统----------");
    puts("-----------------------------");

    char chose;
    scanf(" %c", &chose);
//    while (getchar() != '\n');
    // 清理输入缓冲区，移除多余的字符，包括换行符
    switch (chose) {
    case '1':
      RequestInfo(CLIENT_LOGIN, client_fd); //调用发送操作函数login信息给服务器
      if (Returnquest(LOGIN_ALLOW, client_fd)) {
	char *backstatus=userLogin(client_fd);
        if (strcmp(backstatus, ADMIN_LOGIN) == 0) {
          adminMenu(client_fd); //调用管理员菜单
        } else if (strcmp(backstatus, NORMALUSER_LOGIN) == 0) {
          normalUserMenu(client_fd); //调用普通用户菜单
        } else {
        }
      }
      break;
    case '2':
      isRun = false;
      break;
    default:
      puts("请输入正确选项");
      break;
    }
  } while (isRun);
}



//发送操作信息
void RequestInfo(char *optype, int client_fd) {
  int writecount;
  if (strcmp(optype, "") == 0) {
    Backerror_Client(REQUEST_ERROR);
    return;
  } else {
	  puts(optype);
    if ((writecount = write(client_fd, optype, 30)) < 0){
      perror("Requestinfo");
	  return;
	  }
	}
  return;
}

//服务器返回是否接受操作函数
bool Returnquest(char *reoptype,int client_fd){
	char comeBacktype[60]="";
	memset(comeBacktype,0,sizeof(comeBacktype));
	int readcount;
	if((readcount=read(client_fd,comeBacktype,sizeof(comeBacktype)))>0){
		comeBacktype[readcount]='\0';
		if(strcmp(comeBacktype,reoptype)==0){
			return true;
		}
	}
	return false;
}



//登录函数
char* userLogin(int client_fd) {
  char input[60]="";
  memset(input,0,sizeof(input));
  bool logincount=true;
  while(logincount){
	memset(input,0,sizeof(input));
    printf("请输入你的用户名\n");
    scanf(" %19s", logininfo.username);
    printf("请输入你的密码\n");
    scanf(" %10s", logininfo.password);
  snprintf(input, sizeof(input) - 1, "%s,%s", logininfo.username, logininfo.password);
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
  //判断服务器返回信息
  char reData[100]="";
  reDatawithServer(reData, client_fd);
  if (strcmp(reData, "LOGIN_USERNAME_ERROR") == 0) {
	  Backerror_Client(LOGIN_USERNAME_ERROR);
  }else if(strcmp(reData,"LOGIN_PASSWORD_ERROR")==0){
		Backerror_Client(LOGIN_PASSWORD_ERROR);
  }else if(strcmp(reData,"LOGIN_THREE_TIMES_ERROR")==0){
		Backerror_Client(LOGIN_THREE_TIMES_ERROR);
		logincount=false;
		return "LOGIN_THREE_TIMES_ERROR";
  }else if(strcmp(reData,ADMIN_LOGIN)==0){
		logincount=false;
		return ADMIN_LOGIN;
  }else if(strcmp(reData,NORMALUSER_LOGIN)==0){
		logincount=false;
		return NORMALUSER_LOGIN;
  }else if(strcmp(reData,"USER_ISLOGGED_ERROR")==0){
		Backerror_Client(USER_ISLOGGED_ERROR);
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
	if((loginReadcount=read(client_fd,reData,200))>0){
		reData[loginReadcount]='\0';
		puts(reData);
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
  char input[N]="";
  memset(input,0,sizeof(input));
  bool isregist=true;
  do{
  puts("请输入新账户：字母和数字组成且不能以数字开头");
  scanf(" %s", u.account);
  if (strcmp(u.account, "admin") == 0 || !isValidInputaccount(u.account)) {
    Backerror_Client(CHANGE_ILLEGAL_ERROR);
	puts("-------------------------------------");
	continue;
  }
  puts("请输入名字");
  scanf(" %s", u.name);
  //密码需满足条件
  puts("请输入密码:必须是字母、数字和下划线组成，位数必须是6-10位");
  scanf(" %s", u.pass);
  if (!isValidInput(u.pass)) {
    Backerror_Client(CHANGE_ILLEGAL_ERROR);
	puts("-------------------------------------");
    continue;
  }
  isregist=false;
  }while(isregist);
  snprintf(input, sizeof(input) - 1, "%s,%s,%s ", u.account, u.name, u.pass);
  int writecount;
  if ((writecount = write(client_fd, input, sizeof(input))) < 0) {
    Backerror_Client(SERVER_COMBACK_ERROR);
    return;
  }
  int readcount;
  char recvData[80];
  if ((readcount = read(client_fd, recvData, sizeof(recvData) - 1)) > 0) {
    recvData[readcount] = '\0';
    if (strcmp(recvData, REGISTER_SUCCESS) == 0) {
	  return;
    } else if (strcmp(recvData, "REGISTER_EXIST_ERROR") == 0) {
      Backerror_Client(REGISTER_EXIST_ERROR);
	  return;
    } else if (strcmp(recvData, "REGISTER_OTHER_ERROR") == 0) {
      Backerror_Client(REGISTER_OTHER_ERROR);
	  return;
    }
  } else {
    Backerror_Client(SERVER_COMBACK_ERROR);
  }
}



//删除用户信息
void Delete(int client_fd) {
  char Account[30]="";//定义变量
  memset(Account,0,sizeof(Account));
  bool isdelete=true;
  do{
  puts("请输入你要删除的账户");
  scanf(" %s", Account);
  if (strcmp(Account, "admin") == 0) {
    Backerror_Client(CHANGE_ILLEGAL_ERROR);
	puts("------------------------------------");
	continue;
  }
  isdelete=false;
  }while(isdelete);
  //判断是否是admin
  int writecount;
  if ((writecount = write(client_fd, Account, sizeof(Account))) < 0) {
    perror("Delete()");
    return;
  }

  int readcount;
  char recvData[200]="";
  memset(recvData,0,sizeof(recvData));
  if ((readcount = read(client_fd, recvData, sizeof(recvData) - 1)) > 0) {
    recvData[readcount] = '\0';
    if (strcmp(recvData, DELETE_SUCCESS) == 0) {
      puts("删除用户成功");
	  return;
    } else if (strcmp(recvData, "DELETE_NOTEXIST_ERROR") == 0) {
      Backerror_Client(DELETE_NOTEXIST_ERROR);
	  return;
    } else {
      Backerror_Client(DELETE_OTHER_ERROR);
	  return;
    }
  } else {
    Backerror_Client(SERVER_COMBACK_ERROR);
	return;
  }
}



//修改用户信息
void ChangeInfo(char *usertype, int client_fd) {
	puts("ok2");
  User u = {0}; //定义一个user类型结构体变量
  char input[N]="";
  bool ischange=true;
  do{
  if (strcmp(usertype, CHANGE_INFO) == 0) {
    puts("请输入要修改的账户");
    scanf(" %s", u.account); //不能是admin
    if (strcmp(u.account, "admin") == 0) {
      Backerror_Client(CHANGE_ILLEGAL_ERROR);
	puts("-------------------------------------");
      continue;
    }
  } else {
    strcpy(u.account, logininfo.username);
  }
  puts("请输入要修改的名字");
  scanf(" %s", u.name);
  puts("请输入密码:必须是字母、数字和下划线组成，位数必须是6-10位");
  scanf(" %s", u.pass);
  if (!isValidInput(u.pass)) {
    Backerror_Client(CHANGE_ILLEGAL_ERROR);
	puts("-------------------------------------");
    continue;
  }
  puts("请输入要修改的性别");
  scanf(" %s", u.sex);
  puts("请输入要修改的年龄");
  scanf(" %d", &u.age);
  puts("请输入要修改的地址");
  scanf(" %s", u.address);
  ischange=false;
  }while(ischange);
  snprintf(input, sizeof(input) - 1, "%s,%s,%s,%s,%d,%s", u.account, u.name,
           u.pass, u.sex, u.age, u.address);
  int writecount;
  if ((writecount = write(client_fd, input, sizeof(input))) < 0) {
    perror("Changeinfo");
    return;
  }
  int readcount;
  char recvData[200];
  if ((readcount = read(client_fd, recvData, sizeof(recvData) - 1)) > 0) {
    recvData[readcount] = '\0';
    if (strcmp(recvData, CHANGE_SUCCESS) == 0) {
      puts("修改成功");
	  return;
    } else if (strcmp(recvData, "CHANGE_NOTEXIST_ERROR") == 0) {
      Backerror_Client(CHANGE_NOTEXIST_ERROR);
	  return;
    } else if (strcmp(recvData, "CHANGE_OTHER_ERROR") == 0) {
      Backerror_Client(CHANGE_OTHER_ERROR);
	  return;
    }
  } else {
    Backerror_Client(SERVER_COMBACK_ERROR);
  }
  return;
}



//查询用户信息
void Queryinfo(char *usertype,int client_fd){
	int writecount;
	char username[30]="";
	puts(usertype);
	if(strcmp(usertype,QUERY_INFO)==0){
		puts("请输入你要查询的账户");
		scanf(" %s",username);		
	}else{
		strcpy(username,usertype);
	}
	puts(username);
	if((writecount=write(client_fd,username,sizeof(username)))<0){
			perror("QueryOpration");
			return;
	}
	puts("正在查询用户信息");
	int readcount;
	char comebackdata[MAXSIZE]="";
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
	char username[30]="";
	memset(username,0,sizeof(username));
	if(strcmp(usertype,QUERY_OPRATION)==0){
		puts("请输入你要查询的账户");
		scanf(" %s",username);		
	}else{
		strcpy(username,usertype);
	}
	if(writecount=write(client_fd,username,sizeof(username))<0){
			perror("QueryOpration");
			return;
	}
	puts("正在查询日志");
	int readcount;
	char comebackdata[MAXSIZE]="";
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
  char input[N]="";
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

//判断密码是否符合
bool isValidInput(const char *str) {
    int len = strlen(str);

    // 检查位数是否在 6 到 10 之间
    if (len < 6 || len > 10) {
        return false;
    }

    // 逐个检查字符串中的字符
    for (int i = 0; str[i] != '\0'; ++i) {
        // 如果字符不是字母、数字或下划线，返回错误
        if (!isalnum(str[i]) && str[i] != '_') {
            return false;
        }
    }

    return true; // 字符串符合要求，返回正确
}

//判断账号是否符合要求
bool isValidInputaccount(const char *str) {
    // 检查第一个字符是否是数字
    if (isdigit(str[0])) {
        return false; // 如果以数字开头，返回错误
    }

    // 逐个检查字符串中的字符
    for (int i = 0; str[i] != '\0'; ++i) {
        // 如果字符不是字母或数字，返回错误
        if (!isalnum(str[i])) {
            return false;
        }
    }

    return true; // 字符串符合要求，返回正确
}


//isalnum()如果字符是字母（即 A-Z 或 a-z）或数字（即 0-9），则该函数返回非零值，否则返回 0。



















