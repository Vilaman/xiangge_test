/***********************************************************************************
# File Name:    数据库日志写入.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月14日 星期二 11时48分07秒
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



void saveLog(Client *c) {
  char *ip = inet_ntoa(c->info.sin_addr);
  int port = ntohs(c->info.sin_port);
  //-----日志处理
  //获取时间字符串
  char *type = "LOGIN";
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  char tempstr[30];
  strftime(tempstr, sizeof(tempstr) - 1, "%Y-%m-%d %H:%M:%S", t);
  sqlite3 *db;
  int rc;
  char *err_message;
  rc=sqlite3_open("test.db",&db);
  if(rc!=SQLITE_OK){
	perror("open()");
	sqlite3_free(err_message);
	return;
  }
  char sql[200];
  sprintf(sql,"insert into logs (ip,port,type,time) values ('%s','%d','%s','%s')",ip,port,type,tempstr);
  rc=sqlite3_exec(db,sql,NULL,NULL,&err_message);
  if(rc!=SQLITE_OK){
	perror("exec()");
	sqlite3_free(err_message);
	return;
  }
	int count=sqlite3_changes(db);
	if(count>0){

		puts("写入日志成功");
	}else{

		puts("写入日志失败");
	}
	sqlite3_close(db);

return;

}
