/***********************************************************************************
# File Name:    woniukeeper2.c
# Author:       WENPINGXIANG
# Mail:         630556320@qq.com
# Created Time: 2024年04月16日 星期二 04时14分35秒
 **********************************************************************************/

#include <stdio.h>
#include<stdbool.h> 
#include<string.h> 
#include<stdlib.h> 
char username[10][16]={""};
char password[10][16]={""};
char sex[10][5];
int ages[10];
int logindex=-1;
int len=sizeof(username)/sizeof(username[0]);
int regindex=-1;//用来表示注册的下标

char changePassword(int logindex){
	char inputpasswd[16];
	printf("请输入您的旧密码\n");
	scanf(" %15s",inputpasswd);
	if(strcmp(inputpasswd,password[logindex])==0){
		printf("请输入你的新密码\n");
		scanf(" %15s",password[logindex]);
		printf("密码修改成功\n");
	}else{
		printf("密码不对修改失败\n");
	}
	return 0; 
}
char changeInfo(int logindex){
		printf("请输入您的用户名\n");
		scanf(" %s",username[logindex]);
		printf("请输入您的年龄\n");
		scanf(" %d",&ages[logindex]);
		printf("请输入您要修改的性别\n");
		scanf(" %s",sex[logindex]);
		printf("您的信息已修改为%s %s %d\n",username[logindex],sex[logindex],ages[logindex]);
	return username[logindex],sex[logindex],ages[logindex];
}

char delete(){
	bool isdel=false;
	int delindex=-1;//定义一个下标变量
	char Delusername[16];
	printf("请输入您要删除的用户名\n");
	scanf(" %15s",Delusername);
	for(int i=0;i<len;i++){
		if(strcmp(username[i],Delusername)==0){
			delindex=i;
			isdel=true;
		}
	}
	if(isdel){
	strcpy(username[delindex],"");
	strcpy(password[delindex],"");
	strcpy(sex[delindex],"");
	ages[delindex]=0;
	printf("删除成功\n");
	}else{
		printf("用户名不存在\n");
	}
}
		


void showAll(){
	for(int i=0;i<len;i++){
		if(strcmp(username[i],"")!=0){
			printf("序号%d,用户名：%s密码：%s,年龄：%d,性别：%s\n",i,username[i],password[i],ages[i],sex[i]);
		}
	}
}

bool userIsexsit(char* name){
	bool userisexist=true;
	for(int i=0;i<len;i++){
		if(strcmp(username[i],name)==0){
			userisexist=false;
		}
	}
	return userisexist;
}



bool isExsit(){
	//判断登录前有没有用户名存在
	bool isnotExistuser=false;
	for(int i=0;i<len;i++){
		if(strcmp(username[i],"")!=0){
			isnotExistuser=true;
		}
	}
	return isnotExistuser;
}

int reg(){
	for(int i=0;i<len;i++){
		char name[16];
		printf("请输入您要注册的用户名\n");
		scanf(" %s",name);
		bool Userisnotexist=userIsexsit(name);
		if(Userisnotexist){
			regindex+=1;
			strcpy(username[regindex],name);
			printf("请输入您的密码\n");
			scanf(" %15s",password[regindex]);
			printf("请输入您的年龄\n");
			scanf(" %d",&ages[regindex]);
			printf("请输入您的性别\n");
			scanf(" %4s",sex[regindex]);
			printf("序号%d,用户%s.%s,%d,%s,注册成功\n",regindex,username[regindex],password[regindex],ages[regindex],sex[regindex]);
			break;
		}else{
			printf("您输入的用户名已存在注册失败\n");
			break;
		}
	}
	return 0;
}


int menu2(){
	bool isrun=1;
	printf("----------woniu keeper二级菜单-----------\n");
	do{
	printf("1.刷卡\n");
	printf("2.查询所有用户\n");
	printf("3.修改个人密码\n");
	printf("4.修改个人信息\n");
	printf("5.删除用户\n");
	printf("6.返回上一级\n");
	int act;
	scanf(" %d",&act);
	switch(act){
	case 1:
		printf("请刷卡\n");
		break;
	case 2:
		showAll();
		break;
	case 3:
		changePassword(logindex);
		break;
	case 4:
		changeInfo(logindex);
		break;
	case 5:
		delete();
		break;
	case 6:
		isrun=0;
		}
		}while(isrun);
}
int login(){
	bool Islog=false;
	char iusername[16];
	char ipasswd[16];
	printf("请输入您的用户名\n");
	scanf(" %15s",iusername);
	printf("请输入您的密码\n");
	scanf(" %15s",ipasswd);
	int count=0;
	for(int i=0;i<len;i++){
		if(strcmp(iusername,username[i])==0 && strcmp(ipasswd,password[i])==0){
			Islog=true;
			logindex=i;
			break;
		}
	}

	if(Islog){
		printf("登陆成功\n");
		menu2();
		}else{
			printf("输入错误请先注册，回到上级菜单\n");
		}
	return 0;
}
	

int menu1(){
		printf("---------woniu keeper-----------\n");
	do{
		printf("1.登录系统\n");
		printf("2.注册新用户\n");
		printf("3.退出系统\n");
		int opt;
		scanf(" %d",&opt);
		switch(opt){
			case 1:
				if(isExsit()){
				login(); 
				}else{
					printf("当前没有用户请您先注册\n");
				}
				break; 
			case 2:
				reg();   
				break ;
			case 3:
				return 0;
			default:
				printf("请输入正确的选项\n");
				break;
		}
		
		}while(1);
}



int main(){
	
	menu1();
    return 0;
}
