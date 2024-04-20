/***********************************************************************************
# File Name:    woniukeeper.c
# Author:       WENPINGXIANG
# Mail:         630556320@qq.com
# Created Time: 2024年04月15日 星期一 03时59分04秒
 **********************************************************************************/

#include <stdio.h>
#include<stdbool.h> 
#include<string.h> 
#include<stdlib.h> 


int main(){
	char username[15];
	char passwd[15];
	char deusername[15];
	char depasswd[15];
	char inputpasswd[15];
	int age;
	char sex;
	bool isrun=true;
	while(1){
		printf("---------woniu keeper-----------\n");
		printf("1.登录系统\n");
		printf("2.注册新用户\n");
		printf("3.退出系统\n");
		int opt;
		scanf(" %d",&opt);
		switch(opt){
			case 1:
				if(strcmp(deusername,"")==0){
					printf("当前没有用户请先注册\n");
					break;
				}else{

	
				printf("请输入您的用户名\n");
				scanf(" %s",username);
				printf("请输入您的密码\n");
				scanf(" %s",passwd);
				if(strcmp(username,deusername)==0 && strcmp(passwd,depasswd)==0){
					printf("登陆成功\n");
					while(isrun){
						printf("----------woniu keeper二级菜单-----------\n");
						printf("1.刷卡\n");
						printf("2.查询所有用户\n");
						printf("3.修改密码\n");
						printf("4.返回上一级\n");
						int act;
						char newpasswd[15];
						scanf(" %d",&act);
						switch(act){
							case 1:
								printf("请刷卡\n");
								break;
							case 2:	
								printf("用户名：%s,年龄：%d,性别：%c\n",username,age,sex);
								break;
							case 3:
								printf("请输入您的旧密码；\n");
								scanf(" %s",inputpasswd);
								if(strcmp(inputpasswd,passwd)!=0){
									printf("密码错误\n");
									break;
								}else{
									printf("请输入您的新密码\n");
									scanf(" %s",newpasswd);
									strcpy(depasswd,newpasswd);
									printf("修改成功");
									break;
								}
							case 4:
								isrun=false;
						}

					}
					
				}else{
					printf("用户名或密码错误\n");
					break;
				}
				}break;
			case 2:
				printf("请输入您的用户名\n");
				scanf(" %s",deusername);
				printf("请输入您的密码\n");
				scanf(" %s",depasswd);
				printf("请输入您的年龄\n");
				scanf(" %d",&age);
				printf("请输入您的性别F/M\n");
				scanf(" %c",&sex);
				printf("注册成功\n");
				break;
			case 3:
				return 0;
		}
	}
    return 0;
}
