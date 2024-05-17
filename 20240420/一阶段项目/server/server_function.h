/***********************************************************************************
# File Name:    server_function.h
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月14日 星期二 15时34分45秒
 **********************************************************************************/
#ifndef SERVER_FUNCTION_H
#define SERVER_FUNCTION_H
#include<stdbool.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>


typedef int (*QueryData)(void *, int , char **, char **);



typedef struct Client{
	struct sockaddr_in info;//客户端信息
	int client_fd;//套接字
}Client;

typedef struct Userinfo{
char name[30];
char account[16];
char pass[16];
char sex[5];
int age;
bool isLogin;
bool isAdmin;
}User;


void *handleClient(void *args);

//接收客户端操作函数
char *confirm_Op(Client *c);

//返回接受客户端操作传入操作
void writedata(char *optype,Client *c);

//主操作函数块
void server_Mainop(Client *c);

//登录模块
void loginMoudle(Client *c);

//注册模块
void RegisterMoudle(Client *c);

//删除模块
void DeleteMoudle(Client *c);

//修改信息模块
void ChangeMoudle(Client *c);

//查看信息模块
void QueryInfoMoudle(Client *c);

//查看历史操作模块
void QueryOpMoudle(Client *c);

//数据库增删改模块
bool Sqlop(char *sql);

//数据库查询接口
char  *SqlQuery(char *sql,QueryData callback);

//sql回调函数
int QueryAllData(void *data, int argc, char **argv, char **azColName);

//查询单个user数据并返回值
int QuerySLData(void *data, int argc, char **argv, char **azColName);

//添加日志记录模块API
void AddOplog(Client *c, char *optype, char *content,char *user);

//客户端注销
void Userlogout(Client *c,char *user);






























#endif

