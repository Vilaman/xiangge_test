/***********************************************************************************
# File Name:    client_error.h
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月14日 星期二 16时11分36秒
 **********************************************************************************/
#ifndef CLIENT_ERROR_H
#define CLIENT_ERROR_H

//定义相关error枚举

enum client_error{
	LOGIN_USERNAME_ERROR,//用户名输入错误
	LOGIN_PASSWORD_ERROR,//用户密码输入错误
	LOGIN_THREE_TIMES_ERROR,//用户密码超过三次错误
	LOGIN_FORMAT_ERROR,//用户输入为空或格式错误
	LOGIN_CHOSER_ERROR,//登录选择功能错误
	REGISTER_FORMAT_ERROR,//注册格式错误
	REGISTER_OTHER_ERROR,//注册其他错误
	DELETE_NOTEXIST_ERROR,//管理员删除员工不存在
	DELETE_OTHER_ERROR,//删除其他错误
	CHANGE_NOTEXIST_ERROR,//修改用户不存在错误
	CHANGE_OTHER_ERROR,//修改用户其他错误
	SERVER_CONNECT_ERROR//服务器连接错误
};

//定义error返回函数
char *Backerror_Client(int);






























#endif

