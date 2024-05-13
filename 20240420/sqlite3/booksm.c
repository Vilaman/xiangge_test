/***********************************************************************************
# File Name:    booksm.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月13日 星期一 16时49分39秒
 **********************************************************************************/

#include <stdio.h>
#include<string.h> 
#include<stdbool.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<pthread.h>
#include<semaphore.h>
#include<sys/wait.h>
#include<sys/msg.h>
#include<sqlite3.h>

void insert();
void search();
int showsearch(void *NotUsed,int argc, char **argv,char **azColName);
void searchLike();
void del();
void update();
int showone(void *NotUsed, int argc, char **argv, char **azColName);

sqlite3 *db;
int rc;
char *err_message;

typedef struct books{
	int id;
	char title[20];
	char author[10];
	int year;
}book;

int main() {
  //链接数据库
  rc = sqlite3_open("test.db", &db);
  if (rc != SQLITE_OK) {
    printf("链接数据库失败\n");
    exit(0);
  }
  int choose = -1;
  int isRun = 1; //控制循环执行
  while (isRun) {
	printf("1.插入数据\n");
	printf("2.查找数据\n");
	printf("3.根据名字查询数据\n");
	printf("4.根据ID删除数据\n");
	printf("5.更新数据\n");
	printf("6.退出\n");
    scanf(" %d", &choose);
    switch (choose) {
    case 1:
      insert();
      break;
    case 2:
      search();
      break;
    case 3:
      searchLike();
      break;
    case 4:
      del();
      break;
    case 5:
      update();
      break;
    case 6:
      isRun = 0;
      break;
    default:
      printf("输出错误，请重新输入\n");
      break;
    }
  }
  return 0;
}

void insert() {
  book b = {0};
  char sql[100];
  puts("请输入图书名");
  scanf(" %s", b.title);
  puts("请输入作者");
  scanf(" %s", b.author);
  puts("请输入发行年份");
  scanf(" %d", &b.year);
  sprintf(sql,"insert into books (title,author,published_year) values ('%s','%s',%d)",b.title,b.author,b.year);
  puts(sql);
  rc = sqlite3_exec(db, sql, NULL, NULL, &err_message);
  if (rc != SQLITE_OK) {
    perror("exec()");
    exit(-1);
	sqlite3_free(err_message);
  }
  int count=sqlite3_changes(db);
  if(count>0){

	  puts("插入成功");
  }else{

	  puts("插入失败");
  }

  return;
}


void search() {
	char *sql="select * from books";
	rc=sqlite3_exec(db,sql,showsearch,NULL,&err_message);
	if(rc!=SQLITE_OK){

		perror("exec()");
		exit(-1);
		sqlite3_free(err_message);
	}
return;
}


int showsearch(void *NotUsed, int argc, char **argv, char **azColName) {
	if(argc>0){

		for(int i=0;i<argc;i++){

			printf("%s  ",argv[i]);
		}
		printf("\n");
	}
	return 0;
}


int showone(void *NotUsed, int argc, char **argv, char **azColName) {
  char *name = (char *)NotUsed;
  int j = -1;
  if (argc > 0) {
    for (int i = 0; i < argc; i++) {
      if (strcmp(argv[i], name) == 0) {
        j = i;
      }
    }
  }

	printf("%d\n",j);
  if (j == -1) {
    puts("没有查询到数据");
  } else {

    for (int k = 0; k < j + 3; k++) {
      printf("%s  ", argv[k]);
    }
	printf("\n");
  }

  return 0;
}


void searchLike() {
  char sql[100] = {0};
  char name[30];
  puts("请输入要查找的书名");
  scanf(" %s", name);
  sprintf(sql, "select * from books where title='%s'", name);
  rc = sqlite3_exec(db, sql,showone,name, &err_message);
  if (rc != SQLITE_OK) {
    perror("exec()");
    return;
  }
  return;
}


void del() {

  char sql[100];
  int id;
  printf("请输入要删除的ID\n");
  scanf(" %d", &id);
  sprintf(sql, "delete from books where book_id=%d", id);
  rc = sqlite3_exec(db, sql, NULL, NULL, &err_message);
  if (rc != SQLITE_OK) {

    perror("exec()");
	return;
  }
  int count = sqlite3_changes(db);
  if (count > 0) {

    puts("删除成功");
  } else {

    puts("删除失败");
  }

  return;
}



void update(){
	char sql[100];
	book b;
	puts("请输入要修改图书的id号");
	scanf(" %d",&b.id);
	puts("请输入要修改图书的书名");
	scanf(" %s",b.title);
	puts("请输入要修改图书的作者名");
	scanf(" %s",b.author);
	puts("请输入要修改图书的发行年份");
	scanf(" %d",&b.year);
	sprintf(sql,"update books set title='%s',author='%s',published_year=%d where book_id=%d",b.title,b.author,b.year,b.id);
	rc=sqlite3_exec(db,sql,NULL,NULL,&err_message);
	if(rc!=SQLITE_OK){
		perror("exec()");
		return;
	}
	int count=sqlite3_changes(db);
	if(count>0){

		puts("update success");
	}else{

		puts("update fail");
	}

return;
}
