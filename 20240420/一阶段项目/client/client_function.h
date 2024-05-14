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
	int id;
	char name[20];
	char pass[11];
	char sex[6];
	int age;
	char post[10];
	double salary;
	char phone[12];
}User;



/******************
*功能选择函数*
*******************/

//登录选项
void userLogin();
//判断登录用户是否为管理员
bool isAdmin_login();
//管理员操作菜单
char *adminMenu();
//普通用户菜单
char *normalUserMenu();



/*********************
 * 管理员操作菜单函数*
 * *******************/

//注册新用户
char *_Register();
//删除用户
char *Delete();
//修改用户信息
char *ChangeInfo();
//查询用户信息
char *Queryinfo();
//查询用户操作记录
char *QueryOpration();

/*******************
 * 普通用户操作菜单*
 * ****************/

//普通用户查询信息
char *UserQuery();
//普通用户修改信息
char *UserChangeself();
//普通用户查看自身操作历史
char *UserQueryopation();





































#endif

