/***********************************************************************************
# File Name:    client_function.h
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月14日 星期二 16时10分12秒
 **********************************************************************************/
#ifndef CLIENT_FUNCTION_H
#define CLIENT_FUNCTION_H
//定义用户结构体
typedef struct Userinfo{
	char account[20];
	char name[20];
	char pass[20];
	char sex[6];
	int age;
	char address[15];
	int isLogin;
	int isAdmin;
}User;

typedef struct Info{

	char username[20];
	char password[20];
}Info;


/******************
*功能选择函数*
*******************/

//选择功能
void mainmenu(bool isRun,int client_fd);
//登录选项
char *userLogin(int client_fd);
//发送操作信息
void RequestInfo(char *optype,int client_fd);

//判断登录用户是否为管理员
bool isAdmin_login(char *comeBackData);
//管理员操作菜单
void adminMenu(int client_fd);
//普通用户菜单
void normalUserMenu(int client_fd);
//接收服务器返回是否接受操作函数
bool Returnquest(char *reoptype,int client_fd);
//判断登录服务器返回函数
void reDatawithServer(char *reData,int client_fd);




/*********************
 * 管理员操作菜单函数*
 * *******************/

//注册新用户
void _Register(int client_fd);
//删除用户
void Delete(int client_fd);
//修改用户信息
void ChangeInfo(char *usertype, int client_fd);
//查询用户信息
void Queryinfo(char *usertype,int client_fd);
//查询用户操作记录
void QueryOpration(char *usertype,int client_fd);


//判断密码是否符合
bool isValidInput(const char *str);

//判断账号是否符合要求
bool isValidInputaccount(const char *str);

































#endif

