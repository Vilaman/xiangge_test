/***********************************************************************************
# File Name:    test.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月16日 星期四 12时02分48秒
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



int printed = 0; // 标志变量，用于判断是否已经打印了列名
// 回调函数，在sqlite3_exec中被调用
int callback(void *data, int argc, char **argv, char **azColName) {
char* ret=(char *)data;
  if (!printed) {
    for (int i = 0; i < argc; i++) {
      printf("%s ", azColName[i]); // 打印列名
    }
    printf("\n");

  }
  // 打印数据
  for (int i = 0; i < argc; i++) {
 	strcat(ret, argv[i] ? argv[i] : "NULL");
        strcat(ret, " "); // 添加空格分隔    
printf("%s ", argv[i] ? argv[i] : "NULL");
  }
  printf("\n"); // 添加换行符;
  return 0;
}

int main(int argc, char* argv[]) {
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    char result[400]="";

    // 打开数据库连接
    rc = sqlite3_open("test.db", &db);
    if(rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return(0);
    }

    // 执行查询
    rc = sqlite3_exec(db, "SELECT * FROM books where title='一千'and author='两千';", callback, &result, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
	int size=strlen(result);
        printf("Result: %s %d\n", result,size); // 输出查询结果
    }
    // 关闭数据库连接
    sqlite3_close(db);
    return 0;
}
