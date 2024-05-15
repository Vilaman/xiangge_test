/***********************************************************************************
# File Name:    client_info.h
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月14日 星期二 16时16分50秒
 **********************************************************************************/
#ifndef CLIENT_INFO_H
#define CLIENT_INFO_H

#define CLIENT_LOGIN "CLIENT_LOGIN"           //发送需要登录信号

#define NORMALUSER_LOGIN "NORMALUSER_LOGIN"   //非管理员用户登录

#define ADMIN_LOGIN "ADMIN_LOGIN"             //管理员登录登录

#define LOGIN_ALLOW "LOGIN_ALLOW"            //服务器返回允许登录

#define USER_REGISTER "USER_REGISTER"         //注册用户

#define REGISTER_ALLOW "REGISTER_ALLOW"       //服务器允许注册

#define REGISTER_SUCCESS "REGISTER_SUCCESS"   //服务器返回注册成功

#define DELETE_USER "DELETE_USER"             //删除用户

#define DELETE_ALLOW "DELETE_ALLOW"           //服务器允许删除

#define DELETE_SUCCESS "DELETE_SUCCESS"       //服务器返回删除成功

#define CHANGE_INFO "CHANGE_INFO"             //管理员修改用户信息

#define CHANGE_ALLOW "CHANGE_ALLOW"           //服务器允许修改信息

#define CHANGE_SUCCESS "CHANGE_SUCCESS"       //服务器返回修改信息成功

#define QUERY_INFO "QUERY_INFO"               //管理员查看用户信息

#define QUERYINFO_ALLOW "QUERYINFO_ALLOW"     //服务器允许查看信息

#define QUERY_OPRATION "QUERY_OPRATION"       //管理员查看操作历史

#define QUERYOP_ALLOW "QUERYOP_ALLOW"         //服务器允许查询日志

#define NORMAL_QUERYINFO "NORMAL_QUERYINFO"   //普通用户查看基本信息

#define NORMALQI_ALLOW "NORMALQI_ALLOW"       //服务器允许普通用户查看信息

#define NORMAL_CHANGEINFO "NORMAL_CHANGEINFO" //普通用户修改信息

#define NORMALCH_ALLOW "NORMALCH_ALLOW"       //服务器允许普通用户修改信息

#define NORMAL_QUERYOP "NORMAL_QUERYOP"       //普通用户查看操作日志

#define NORMALQOP_ALLOW "NORMALQOP_ALLOW"     //服务器同意普通用户查看日志

#define CONNECT_SUCCESS "CONNECT_SUCCESS"     //连接服务器成功




#define PORT 8088                             //定义端口信息
#define N 200        //定义客户端发送数据字节数
#define MAXSIZE 1024 //定义接收最大字节数
#define M 30   //定义操作发送字节数







































#endif
