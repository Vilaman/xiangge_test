/***********************************************************************************
# File Name:    client_error.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月14日 星期二 16时12分45秒
 **********************************************************************************/

#include <stdio.h>
#include<string.h> 
#include<stdbool.h>
#include<stdlib.h>
#include "client_error.h"

//定义一个枚举返回字符串
enum client_error Client_error;


void Backerror_Client(int Client_error){

	switch(Client_error){
		case LOGIN_USERNAME_ERROR: puts("LOGIN_USERNAME_ERROR"); break;
		case LOGIN_PASSWORD_ERROR: puts("LOGIN_PASSWORD_ERROR"); break;
		case LOGIN_THREE_TIMES_ERROR: puts("LOGIN_THREE_TIMES_ERROR"); break;
		case LOGIN_FORMAT_ERROR: puts("LOGIN_FORMAT_ERROR"); break;
		case LOGIN_CHOSE_ERROR: puts("LOGIN_CHOSE_ERROR"); break;
		case REGISTER_FORMAT_ERROR: puts("REGISTER_FORMAT_ERROR"); break;
		case REGISTER_OTHER_ERROR: puts("REGISTER_OTHER_ERROR"); break;
		case DELETE_NOTEXIST_ERROR: puts("DELETE_NOTEXIST_ERROR"); break;
		case DELETE_OTHER_ERROR: puts("DELETE_OTHER_ERROR"); break;
		case CHANGE_NOTEXIST_ERROR: puts("CHANGE_NOTEXIST_ERROR"); break;
		case CHANGE_OTHER_ERROR: puts("CHANGE_OTHER_ERROR"); break;
		case SERVER_COMBACK_ERROR: puts("SERVER_COMBACK_ERROR"); break;
		case REQUEST_ERROR:puts("REQUEST_ERROR");break;
		case SELECT_OPERATION_ERROR:puts("SELECT_OPERATION_ERROR");break;
		case QUERYINFO_FAIL:puts("QUERINFO_FAIL_ERROR");break;
		case QUERYOP_FAIL:puts("QUERYOP_FAIL_ERROR");break;
		case REGISTER_EXIST_ERROR:puts("REGISTER_EXIST_ERROR");break;
		case CHANGE_ILLEGAL_ERROR:puts("CHANGE_ILLEGAL_ERROR");break;
		case DELETE_ILLEGAL_ERROR:puts("DELETE_ILLEGAL_ERROR");break;
		default:puts("UNDEFINED ERROR");break;
	}
}
