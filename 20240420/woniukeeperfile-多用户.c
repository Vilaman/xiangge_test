/***********************************************************************************
# File Name:    woniukeeperfile-多用户.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年04月26日 星期五 19时26分25秒
 **********************************************************************************/

#include <stdio.h>
#include<string.h> 
#include<stdbool.h>
#include<stdlib.h>
#include"mySLNode.h"
#include<time.h>
#define N 50

int line=0;//定义文件是否存在用户行数
Node *user=NULL; //定义单链表指针
int logind=-1;

void menuone();
void menutwo();
bool isExistuser();
bool isExistinTXT(char *newname, int line);
int login();
int UserReadandInit();
void UserWrite(User *u);
void regIster();
void DelUser();
void writeTolog(char *status);




int main() {
  UserReadandInit();
  menuone();
//  printf("这是退出函数后\n");
//  showNode(user);
  Nodefree(&user);

  return 0;
}


void menuone() {

  bool isRun = true;
  printf("欢迎来到蜗牛门禁系统\n");
  do {
    printf("-----------------------------------------\n");
    printf("1.登录系统   2.注册系统     3.退出系统\n");
    printf("-----------------------------------------\n");
    char opt;
    scanf(" %c", &opt);
    switch (opt) {
    case '1':
      login();
      break;
    case '2':
      regIster();
      break;
    case '3':
      isRun = 0;
      break;
    default:
      printf("请输入正确的选项\n");
      break;
    }
  } while (isRun);
  return;
}


void menutwo() {
  bool isRuntwo = true;
  char *statusshow="view-user";
  char *statuslogout="logout";
  printf("欢迎来到二级菜单\n");
  do {
    printf("-----------------------------------------\n");
    printf("1.查看信息   2.修改密码     3.修改信息\n");
    printf("4.删除用户   5.退出系统\n");
    printf("-----------------------------------------\n");
    char seLe;
    scanf(" %c", &seLe);
    switch (seLe) {

    case '1':
      showNode(user);
	writeTolog(statusshow);
      break;
    case '2':
      //    ChangePass();
      break;
    case '3':
      //   ChangeData();
      break;
    case '4':
//	  printf("下标 %d\n",logind);
      DelUser();
      break;
    case '5':
      isRuntwo = false;
	  writeTolog(statuslogout);
      return;
    default:
      printf("请输入正确的选项\n");
      break;
    }

  } while (isRuntwo);
  return;
}

//通过文件行数来判断当前是否有用户存在
bool isExistuser() {

  if (line == 0) {
    return true;
  }
  return false;
}



bool isExistinTXT(char *newname, int line) {
	User u;
  FILE *fp1 = fopen("message.txt", "r");
  if (fp1 == NULL) {
    perror("open file");
    exit(1);
  }
  for (int i = 0; i < line; i++) {
    fscanf(fp1, "%s %s %s %d\n", u.username, u.password, u.sex, &u.age);
    if (strcmp(u.username, newname) == 0) {
      return true;
    }
  }
  fclose(fp1);
  fp1 = NULL;
  return false;
}




int login() {
	char *status="login";
  char loginname[20];
  char loginpass[20];
//  printf("line:%d\n", line);
  if (isExistuser()) {
    printf("当前没有用户存在请先注册\n");
    return -1;
  }
  printf("请输入用户名\n");
  scanf(" %s", loginname);
  printf("请输入密码\n");
  scanf(" %s", loginpass);
  logind = isDataexist(user, loginname, loginpass);
  //  printf("%d\n", logind);
  if (logind == -1) {

    printf("登录失败\n");
    return -1;
  }
  printf("登录成功\n");
  writeTolog(status);
  menutwo();
  return 0;
}




//读取message.txt里面的用户信息并初始化链表
int UserReadandInit() {
  User *u;

  FILE *readpf = fopen("message.txt", "r");
  if (readpf == NULL) {

    perror("fopen()");
    exit(1);
  }
  char buf[N];
  while (fgets(buf, N, readpf) != NULL) {
    if (buf[strlen(buf) - 1] == '\n')
      line++;
  }
//  printf("Total line : % d\n", line);
  if (line == 0) {

    printf("当前没有用户请先注册\n");
    return 0;
  }
  u = (User *)malloc(line * sizeof(User));
  fseek(readpf, 0, SEEK_SET);//用指针偏移量归位 不然是出于EOF
  for (int i = 0; i < line; i++) {
    fscanf(readpf, "%s %s %s %d\n", u[i].username, u[i].password, u[i].sex,
           &u[i].age);
  //  printf(" 初始化：%s %s %s %d\n", u[i].username, u[i].password, u[i].sex,
    //       u[i].age);
    appendNode(&user, (u + i)); //初始化链表
  }

  fclose(readpf);
  readpf = NULL;
  return line;
}


//写入到message.txt中
void UserWrite(User *u) {
  FILE *writefp = fopen("message.txt", "a");
  if (writefp == NULL) {
    perror("userwrite open file");
    exit(1);
  }
  fprintf(writefp, "%s %s %s %d\n", u->username, u->password, u->sex, u->age);
  fclose(writefp);
  writefp = NULL;
}


//注册模块
void regIster(){
	char regname[20];
	char strlog[60];
	printf("请输入用户名\n");
	scanf(" %s",regname);
	if(isExistinTXT(regname,line)){
	printf("用户名已存在注册失败\n");
	return;
	}
	User *u1;//定义user结构体指针
	u1=(User*)malloc(sizeof(User));
	strcpy(u1->username,regname);
	printf("请输入密码\n");
	scanf(" %s",u1->password);
	printf("请输入性别\n");
	scanf(" %s",u1->sex);
	printf("请输入年龄\n");
	scanf(" %d",&u1->age);
  //  printf("这是添加后结构体的%s %s %s %d\n",u1->username, u1->password, u1->sex, u1->age);
	appendNode(&user,u1);//调用链表追加数据
//printf("这是添加成功后的\n");
//	showNode(user);
	UserWrite(u1);
	line++;

	printf("注册成功\n");
	return;
}





void ChangePass(){



}



void ChageData(){



}


void DelUser() {
  char *statusdelete="delete-user";
  int num = -1;
  printf("请输入你要删除的用户序号\n");
  scanf(" %d", &num);
  DelNode(&user, num);
	  writeTolog(statusdelete);
  line--;
  FILE *fp2 = fopen("message.txt", "w+");
  if (fp2 == NULL) {

    perror("open file");
    exit(1);
  }
  Node *current = user;
  while (current != NULL) { //从当前链表的第一个节点开始向下遍历

    fprintf(fp2, "%s %s %s %d\n", current->data->username,
            current->data->password, current->data->sex, current->data->age);
 //   printf("%s,%s,%s,%d\n", cur->data->username, cur->data->password,
   //        cur->data->sex, cur->data->age);
    current = current->next;
  }
  fclose(fp2);
  fp2 = NULL;
//  showNode(user);
  return;
}

void writeTolog(char *status) {

  FILE *writelogfp = fopen("log.txt", "a+");
  if (writelogfp == NULL) {
    perror("open file");
    exit(-1);
  }
  time_t now;
  char timestr[40];
  now = time(NULL);
  Node *current1=user;
  //通过localtime将time_t转为一个包含时间的结构体并返回指针
  struct tm *nowtime = localtime(&now);
  // printf("月份：%d 天数:%d %d:%d:%d
  // 星期%d",nowtime->tm_mon+1,nowtime->tm_mday,nowtime->tm_hour,nowtime->tm_min,nowtime->tm_sec,nowtime->tm_wday);
  //  char timestr[40];
  strftime(timestr, 40, "%Y-%m-%d %H:%M:%S", nowtime);
  // printf("%d %s\n",len,timestr);
  char *arr = shownodeData(current1, logind);
  //  printf("%s %s %s\n",arr,timestr,status);
  fprintf(writelogfp, "%s %s %s\n", arr, status, timestr);
  fclose(writelogfp);
  writelogfp = NULL;
}





