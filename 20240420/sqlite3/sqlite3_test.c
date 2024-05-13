/***********************************************************************************
# File Name:    sqlite3_test.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月13日 星期一 15时21分23秒
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


int demo(void *NotUsed,int argc,char **argv,char **azColName);
int main() {

  //打开数据库
  sqlite3 *db;
  char *errmsg=0; //保存错误信息
  int ret = sqlite3_open("test.db", &db);
  if (ret != SQLITE_OK) {
    perror("qlite3_open");
    exit(1);
  }
  char *sql = "select * from books"; // sql语句
  //执行sql语句
  int rc =sqlite3_exec(db,sql,demo,NULL,&errmsg);
  if(rc!=SQLITE_OK){

	  perror("sqlite3_exec");
	  exit(1);
  }

  //关闭db
  sqlite3_close(db);

      return 0;
}

int demo(void *NotUsed,int argc,char **argv,char **azColName){

	if(argc>0){
		for(int i=0;i<argc;i++){

		printf("%s=%s\n",azColName[i],argv[i]);
		}
	}
	return 0;
}


