/***********************************************************************************
# File Name:    woniuATMsystem.c
# Author:       WENPINGXIANG
# Mail:         630556320@qq.com
# Created Time: 2024年04月19日 星期五 04时03分38秒
 **********************************************************************************/

#include <stdio.h>
#include<stdbool.h> 
#include<string.h> 
#include<stdlib.h>
#include<errno.h>
#include <unistd.h>

typedef struct ATMsystem{
	char users[16];
	char password[16];
	float money;
}ATM;
int total=5;//定义用户数变量，最开始为五个。

int successindex = -1; //定义登录成功下标
ATM *woniuATM=NULL;//定义一个叫woniuATM的结构体指针变量
bool isExsit();
bool userExistfun(char *newname);
int regists();
void login();
void loginMenu();
void Queryfun();
float Drawfun();
float Savefun();
float Tranferfun();
void Deletefun();


int main(){
	woniuATM=(ATM *)malloc(total*sizeof(ATM));
	loginMenu();

    return 0;
}

//一级菜单页面
void loginMenu() {
  bool isrun = true;
  printf("---------woniuATM system-----------\n");
  do {
	printf("------------------------------------\n");
    printf("1.登录系统\n");
    printf("2.注册新用户\n");
    printf("3.退出系统\n");
	printf("------------------------------------\n");
    char opt;
    scanf(" %c",&opt);
	while (getchar() != '\n'); // 清理输入缓冲区，移除多余的字符，包括换行符
    switch (opt) {
    case '1':
      if (isExsit()) {
        login();
      } else {
        printf("当前没有用户请您先注册\n");
      }
      break;
    case '2':
      regists();
      break;
    case '3':
      isrun = false;
      break;
    default:
      printf("请输入正确的选项\n");
      break;
    }

  } while (isrun);
}


void mainmenu() {
  bool isrun = true;
  printf("-------woniuATM system二级菜单--------\n");
  do {
	printf("-------------------------------------------------\n");
    printf("1.查询余额\n");
	printf("2.取钱\n");
    printf("4.转账\n");
    printf("5.注销账户\n");
    printf("6.返回上一级\n");
    printf("-------------------------------------------------\n");
	char act;
    scanf(" %c", &act);
    switch (act) {
    case '1':
      Queryfun();
      break;
    case '2':
      Drawfun();
      break;
    case '3':
      Savefun();
      break;
    case '4':
      Tranferfun();
      break;
    case '5':
      Deletefun();
      break;
    case '6':
      isrun = 0;
      break;
    default:
      printf("请输入正确的选项\n");
      break;
    }
  } while (isrun);
}



void login() {
  bool islogin = false;
  ATM *tmp = woniuATM;
  char username[16];
  char passwd[16];
  printf("请输入你的用户名\n");
  scanf(" %15s", username);
  printf("请输入你的登录密码\n");
  scanf(" %15s", passwd);
  for (int i = 0; i < total; i++) {
    if (strcmp(woniuATM[i].users, username) == 0 &&
        strcmp(woniuATM[i].password, passwd) == 0) {
      islogin = true;
	  successindex=i;
    }
  }
  if (islogin) {
    printf("登陆成功\n");
	mainmenu();
  }else{
	printf("登录失败\n");
  }
}



int regists() {
  char *newuser;
  printf("请输入你要注册的用户名\n");
  scanf(" %s", newuser);
  bool userIsexist = userExistfun(newuser);
  int regindex = -1;
  if (userIsexist==0 ) {
    printf("用户已存在注册失败\n");
    return 0;
  }
  for (int i = 0; i < total; i++) {
    if (strcmp(woniuATM[i].users, "") == 0) {
      regindex = i;
	break;
    }
  }
//注册用户
  if (regindex != -1) {
    strcpy(woniuATM[regindex].users, newuser);
    printf("请输入您的密码\n");
    scanf(" %s", woniuATM[regindex].password);
    woniuATM[regindex].money = 0;
    printf("注册成功\n");
  } else {//如果用户注册失败则申请内存空间
    printf("注册失败请重新注册\n");
    total = 2 * total;
    ATM *temp = (ATM *)malloc(total * sizeof(ATM));
	if(temp==NULL){
		printf("%s\n",strerror(errno));
		return 0;
	}
    for (int i = 0; i < total / 2; i++) {
      strcpy(temp[i].users, woniuATM[i].users);
      strcpy(temp[i].password, woniuATM[i].password);
      temp[i].money = woniuATM[i].money;
    }
    free(woniuATM);
    woniuATM = temp;
    temp = NULL;
  }
}

//注册前判断用户是否存在，存在则返回0
bool userExistfun(char *newname){
	for(int i=0;i<total;i++){
		if(strcmp(woniuATM[i].users,newname)==0)
			return 0;
	}
	return 1;
}


//判断登录前是否有用户存在
bool isExsit() {
  for (int i = 0; i < total; i++) {
    if (strcmp(woniuATM[i].users, "") != 0) {
      return 1;
    }
  }
  return 0;
}

//查询余额
void Queryfun() {
  printf("您目前的余额为%.2f元\n",woniuATM[successindex].money);
}


//取钱功能
float Drawfun() {
  if (woniuATM[successindex].money > 0) {
    int num = 0;
    printf("请输入您要取的金额\n");
    scanf(" %d", &num);
    if (num <= woniuATM[successindex].money) {
      woniuATM[successindex].money -= num;
      printf("取钱成功，余额为%.2f\n", woniuATM[successindex].money);
	}
	else{
		printf("取款失败，您的余额不足\n");
	}
  }
  else {
    printf("余额不足，取钱失败\n");
  }
  return woniuATM[successindex].money;
}


//存钱功能
float Savefun(){
	float savemoney=0;
	printf("请输入你要存的金额\n");
	scanf(" %f",&savemoney);
	woniuATM[successindex].money+=savemoney;
	printf("存钱成功您目前的余额为%.2f\n",woniuATM[successindex].money);
	return woniuATM[successindex].money;
}

//转账系统
float Tranferfun() {
  char tranferuser[16];
  float tmoney = 0;
  printf("请输入你要转账得用户名\n");
  scanf(" %s", tranferuser);
  //先判断用户名是否存在
  bool userIsnotexist = userExistfun(tranferuser);
  if (userIsnotexist) {
    printf("你输入的用户名不存在转账失败\n");
  } else {
    for (int i = 0; i < total; i++) {
      if (strcmp(woniuATM[i].users, tranferuser) == 0) {
        printf("请输入你转账的金额\n");
        scanf(" %f", &tmoney);
        if (tmoney <= woniuATM[successindex].money) {
          woniuATM[successindex].money -= tmoney;
          woniuATM[i].money += tmoney;
          printf("转账成功，你的余额为%.2f\n", woniuATM[successindex].money);
          return woniuATM[successindex].money, woniuATM[i].money;

        } else {
          printf("您的金额不足,转账失败\n");
        }
      }
    }
  }
}

//删除用户
void Deletefun() {
  if (woniuATM[successindex].money != 0) {
    printf("你的余额%.2f不为零，注销失败\n",woniuATM[successindex].money);
  } else {
    strcpy(woniuATM[successindex].users, "");
    strcpy(woniuATM[successindex].password ,"");
    printf("您的账号已注销，请返回主菜单");
  }
}
