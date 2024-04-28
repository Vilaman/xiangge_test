/***********************************************************************************
# File Name:    test.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年04月27日 星期六 07时44分37秒
 **********************************************************************************/

#include <stdio.h>
#include<string.h> 
#include<stdbool.h>
#include<stdlib.h>
#define N 40
typedef struct User{
char username[15];
char password[16];
char sex[5];
int age;
}User;

int line=0;

int main() {
  User *u=NULL;

  FILE *readpf = fopen("1.txt", "r");
  if (readpf == NULL) {

    perror("fopen()");
    exit(1);
  }
  char buf[N];
  while (fgets(buf, N, readpf) != NULL) {
    if (buf[strlen(buf) - 1] == '\n')
      line++;
  }
  printf("Total line : % d\n", line);
  if (line == 0) {

    printf("当前没有用户请先注册\n");
    return 0;
  }
  u=(User *)malloc(line*sizeof(User));
  fseek(readpf,0,SEEK_SET);
 // for (int i = 0; i < count; i++) {
   int i= fscanf(readpf, "%s %s %s %d\n", u[0].username, u[0].password, u[0].sex, &u[0].age);
    printf(" 初始化：%d %s %s %s %d\n",i,u[0].username, u[0].password, u[0].sex, u[0].age);
//    appendNode(&user, u);//初始化链表
//  }

  fclose(readpf);
  readpf=NULL;
  return line;

	
	
	return 0;
}
