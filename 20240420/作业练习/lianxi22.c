/***********************************************************************************
# File Name:    lianxi22.c
# Author:       WENPINGXIANG
# Mail:         630556320@qq.com
# Created Time: 2024年04月13日 星期六 01时23分55秒
 **********************************************************************************/
#include<string.h>
#include <stdio.h>
#include<stdbool.h> 
int main(){
	int opt;
	char username[20];
	char passwd[20];
	char newuser[20];
	char newpsd[20];
	int count=1;
	char newpad1[20];
	printf("******欢迎来到登录界面*******\n");
	printf("输入1登录系统\n");
	printf("输入2注册新用户\n");
	scanf(" %d",&opt);
	switch(opt){
		case 1:
 			printf("请输入您的用户名\n");
			scanf(" %s",username);
			if(strcmp(username,"admin")!=0){
				printf("用户名输入错误\n");
			}
			if(strcmp(username,"admin")==0){
 				while(count!=0){
 					printf("请输入您的密码\n");
					scanf(" %s",passwd);
					if(strcmp(passwd,"abcd1234")==0){
						printf("登录成功\n");
						return 0;
					}
					else {
 						count+=1;
						if(count>3){
 							printf("超过最大输入次数3\n");
							return 0;
						}
					}
				}
			}
			break;


		case 2:
 			printf("请输入新的用户名\n");
			scanf(" %s",newuser);
			printf("请设置您的密码\n");
			scanf(" %s",newpsd);
			printf("请再次输入密码确认\n");
			scanf(" %s",newpad1);
			if(strcmp(newpsd,newpad1)==0){
				printf("注册成功！\n");
			}else{
				printf("两次密码不一致注册失败\n");
			}

					
				
	}
    return 0;
}
