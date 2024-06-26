/***********************************************************************************
# File Name:    server_function.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月14日 星期二 15时29分56秒
 **********************************************************************************/

#include "server_function.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <semaphore.h>
#include <sqlite3.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include "server_info.h"
#include "pthread_pool.h"




//接收客户端操作函数
char *confirm_Op(Client *c) {
  char data[60];
  memset(data,0,sizeof(data));
  int readcount = read(c->client_fd, data, sizeof(data) - 1);
  if (readcount > 0) {
    data[readcount] = '\0';
  } else if (readcount <= 0) {
    return USER_LOGOUT;
  }
  if (strcmp(data, CLIENT_LOGIN) == 0) { //登录
    writedata(LOGIN_ALLOW, c);
    return CLIENT_LOGIN;
  } else if (strcmp(data, USER_REGISTER) == 0) { //注册
    writedata(REGISTER_ALLOW, c);
    return USER_REGISTER;
  } else if (strcmp(data, DELETE_USER) == 0) { //删除
    writedata(DELETE_ALLOW, c);
    return DELETE_USER;
  } else if (strcmp(data, CHANGE_INFO) == 0 || strcmp(data, NORMAL_CHANGEINFO) == 0) { //修改
    if (strcmp(data, CHANGE_INFO) == 0) {
      writedata(CHANGE_ALLOW, c);
    } else {
      writedata(NORMALCH_ALLOW, c);
    }
    return CHANGE_INFO;
  } else if (strcmp(data, QUERY_INFO) == 0 ||strcmp(data, NORMAL_QUERYINFO) == 0) { //查信息
    if (strcmp(data, QUERY_INFO) == 0) {
      writedata(QUERYINFO_ALLOW, c);
    } else {
      writedata(NORMALQI_ALLOW, c);
    }
    return QUERY_INFO;
  } else if (strcmp(data, QUERY_OPRATION) == 0 ||strcmp(data, NORMAL_QUERYOP) == 0) { //查操作
    if (strcmp(data, QUERY_OPRATION) == 0) {
      writedata(QUERYOP_ALLOW, c);
    } else {
      writedata(NORMALQOP_ALLOW, c);
    }
    return QUERY_OPRATION;
  }
  return NULL;
}


//返回接受客户端操作传入操作
void writedata(char *optype,Client *c){
	int writecount;
	char temp[50]="";
	strcpy(temp,optype);
	if((writecount=write(c->client_fd,temp,strlen(temp)))<=0){
		perror("write()");
		exit(-1);
	}
	return;
}

//主操作函数块
bool server_Mainop(Client *c){
	char *opback=confirm_Op(c);
	if(strcmp(opback,CLIENT_LOGIN)==0){
		loginMoudle(c);
	}else if(strcmp(opback,USER_REGISTER)==0){
		RegisterMoudle(c);
	}else if(strcmp(opback,DELETE_USER)==0){
		DeleteMoudle(c);
	}else if(strcmp(opback,CHANGE_INFO)==0){
		ChangeMoudle(c);
	}else if(strcmp(opback,QUERY_INFO)==0){
		QueryInfoMoudle(c);
	}else if(strcmp(opback,QUERY_OPRATION)==0){
		QueryOpMoudle(c);
	}else if(strcmp(opback,USER_LOGOUT)==0){
		Userlogout(c);
		return false;
	}
	return true;
}



//登录模块
void loginMoudle(Client *c) {
  bool isLogMoudle = true;
  int readcount;
  char acceptdata[60] = "";
  char sql[N];
  char *logaccount;
  char *logpass;
  char *content = "登录系统";
  int count = 1;
  while (isLogMoudle) {
    if ((readcount = read(c->client_fd, acceptdata, sizeof(acceptdata))) > 0) {
      acceptdata[readcount] = '\0';
      logaccount = strtok(acceptdata, ",");
      logpass = strtok(NULL, ",");
    } else {
      return;
    }
    int writecount;
    sprintf(sql,"select * from UserInfo where user_account='%s' and user_isLogin=0;",logaccount);
    char *backdata = SqlQuery(sql, QuerySLData);
    if (strcmp(backdata, "") != 0) {
      memset(sql, 0, sizeof(sql));
      sprintf(sql, "select * from UserInfo where user_account='%s';",logaccount);
      char *backdata = SqlQuery(sql, QuerySLData);
      if (strcmp(backdata, "") != 0) {
        memset(sql, 0, sizeof(sql));
        sprintf(sql,"select * from UserInfo where user_account='%s' and user_password='%s';",logaccount, logpass);
        char *backdata1 = SqlQuery(sql, QuerySLData);
        if (strcmp(backdata1, "") != 0) {
          memset(sql, 0, sizeof(sql));
          count++;
          sprintf(sql,"update UserInfo set user_isLogin=1 where user_account='%s';",logaccount);
          Sqlop(sql);
          AddOplog(c, USER_LOGIN, content, logaccount);
          if (strcmp(logaccount, "admin") == 0) {
            if ((writecount = write(c->client_fd, ADMIN_LOGIN, M)) > 0) {
              strcpy(c->User, logaccount);
              isLogMoudle = false;
              return;
            } else {
            }
          } else {
            if ((writecount = write(c->client_fd, NORMALUSER_LOGIN, M)) > 0) {
              isLogMoudle = false;
              strcpy(c->User, logaccount);
              return;
            } else {
            }
          }
        } else {
          if (count >= 3) {
            if ((writecount =write(c->client_fd, "LOGIN_THREE_TIMES_ERROR", M)) > 0) {
              isLogMoudle = false;
              return;
            }
          }
          if ((writecount = write(c->client_fd, "LOGIN_PASSWORD_ERROR", M)) > 0) {
            count++;
          } else {
          }
        }
      } else {
        if ((writecount = write(c->client_fd, "LOGIN_USERNAME_ERROR", M)) > 0) {
        } else {
          //客户端下线
        }
      }
    }else{
        if ((writecount = write(c->client_fd, "USER_ISLOGGED_ERROR", M)) > 0) {
        } else {
          //客户端下线
        }
	}
  }
}





//注册模块
void RegisterMoudle(Client *c){
	int readcount;
	int writecount;
	char data[210]="";
	char sql[N];
	char *content="注册新用户";

	if((readcount=read(c->client_fd,data,sizeof(data)-1))>0){
		data[readcount]='\0';
	}else{
		return;
	}
	puts(data);
	char *newaccount=strtok(data,",");
	char *newpass=strtok(NULL,",");
	char *newname=strtok(NULL,",");
	sprintf(sql,"insert into UserInfo (user_account,user_password,user_name) values ('%s','%s','%s');",newaccount,newpass,newname);
	if(Sqlop(sql)){
		if((writecount=write(c->client_fd,REGISTER_SUCCESS,M))>0){
			AddOplog(c,USER_REGISTER,content,"admin");
		}else{
			//客户端下线
		}
	}else{
		if((writecount=write(c->client_fd,"REGISTER_EXIST_ERROR",M))<=0){
			//客户端下线
		}
	}
	return;
}



//删除模块
void DeleteMoudle(Client *c) {
  int readcount;
  char acceptdata[D]="";
  memset(acceptdata,0,sizeof(acceptdata));
  char sql[N]="";
  char *content = "删除用户";

  if ((readcount = read(c->client_fd, acceptdata, M - 1)) > 0) {
    acceptdata[readcount] = '\0';
  } else {
    //
    return;
  }
  sprintf(sql, "delete from UserInfo where user_account='%s';", acceptdata);
  int writecount;
  if (Sqlop(sql)) {
	  char temp[50]=DELETE_SUCCESS;
    if ((writecount = write(c->client_fd, temp, sizeof(temp))) <= 0) {
      //客户端下线或未发送成功
    } else {
      AddOplog(c, DELETE_USER, content,"admin");
    }
  } else {
    if ((writecount = write(c->client_fd, "DELETE_NOTEXIST_ERROR", M)) <= 0) {
      //客户端下线或者未发送出
      return;
    }
  }
  return;
}


//修改信息模块
void ChangeMoudle(Client *c) {
  char data[250]="";

  int readcount;
  int writecount;
  char sql[N]="";
  char *content = "修改用户信息";
  if((readcount=read(c->client_fd,data,sizeof(data)-1))>0){
	  data[readcount]='\0';
  }else{
	  //错误处理
	  return;
  }
  char *account=strtok(data,",");
  char *name=strtok(NULL,",");
  char *pass=strtok(NULL,",");
  char *sex=strtok(NULL,",");
  char *age1=strtok(NULL,",");
	int age=atoi(age1);	
  char *address=strtok(NULL,",");
  sprintf(sql,"update UserInfo set user_name='%s',user_password='%s',user_sex='%s',user_age=%d,user_address='%s' where user_account='%s';",name,pass,sex,age,address,account);
  puts(sql);
  if (Sqlop(sql)) {
    if ((writecount = write(c->client_fd, CHANGE_SUCCESS, M)) <= 0) {
      //客户端下线或未发送成功
    } else {
      AddOplog(c, CHANGE_INFO, content,account);
    }
  } else {
    if ((writecount = write(c->client_fd, "CHANGE_NOTEXIST_ERROR", M)) <= 0) {

      //客户端下线或者未发送出
      return;
    }
  }

}





//查看信息模块
void QueryInfoMoudle(Client *c) {
	puts("查询信息");
  char data[MAXSIZE]="";
  int readcount;
  int writecount;
  char recvdata[D]="";
  char sql[N]="";
  char *content = "查询信息";
  if ((readcount = read(c->client_fd,recvdata,sizeof(recvdata) - 1)) > 0) {
    recvdata[readcount] = '\0';
  } else {

    //错误处理
    return;
  }
  sprintf(sql, "select * from UserInfo where user_account='%s';", recvdata);
  strcpy(data, SqlQuery(sql, QueryAllData));
  if (strcmp(data, "") != 0) {
    if ((writecount = write(c->client_fd, data, sizeof(data))) <= 0) {
      //客户端下线或未发送成功
    } else {
      AddOplog(c, QUERY_INFO, content, recvdata);
    }
  } else {
    if ((writecount = write(c->client_fd, "QUERYINFO_FAIL", M)) <= 0) {

      //客户端下线或者未发送出
      return;
    }
  }
}






//查看历史操作模块
void QueryOpMoudle(Client *c) {
  char data[MAXSIZE]="";
  int readcount;
  int writecount;
  char recvdata[D]="";
  char sql[N];
  char *content = "查询操作历史";
  if ((readcount = read(c->client_fd,recvdata,sizeof(recvdata) - 1)) > 0) {
    recvdata[readcount] = '\0';
  } else {

    //错误处理
    return;
  }
  sprintf(sql, "select * from Userlog where op_account='%s';", recvdata);
  strcpy(data, SqlQueryHistory(sql, QueryAllData));
  if (strcmp(data, "") != 0) {
    if ((writecount = write(c->client_fd, data, sizeof(data))) <= 0) {
      //客户端下线或未发送成功
    } else {
      AddOplog(c, QUERY_LOG, content, recvdata);
    }
  } else {
    if ((writecount = write(c->client_fd, "QUERYOP_FAIL", M)) <= 0) {

      //客户端下线或者未发送出
      return;
    }
  }
}




//数据库增删改模块
bool Sqlop(char *sql) {
  sqlite3 *db;
  int rc;
  char *err_message;
  //链接数据库
  rc = sqlite3_open("Database.db", &db);
  if (rc != SQLITE_OK) {
    perror("opendatabase()");
    return false;
  }
  rc = sqlite3_exec(db, sql, NULL, NULL, &err_message);
  if (rc != SQLITE_OK) {
    perror("exec()");
    sqlite3_free(err_message);
    return false;
  }
  int count = sqlite3_changes(db);
  sqlite3_close(db);
  if (count > 0) {
    return true;
  } else {
    return false;
  }
}


//查询日志接口 因为需要空间较大和普通信息区分开
char  *SqlQueryHistory(char *sql,QueryData callback) {
  sqlite3 *db;
  int rc;
  char *err_message;
  char *data=NULL;

  data=malloc(sizeof(char)*MAXSIZE);
  memset(data,0,sizeof(MAXSIZE));
  //链接数据库
  rc = sqlite3_open("Database.db", &db);
  if (rc != SQLITE_OK) {
    perror("opendatabase()");
	sqlite3_free(err_message);
    return NULL;
  }
  rc = sqlite3_exec(db, sql,callback, data, &err_message);
  sqlite3_close(db);
  if (rc != SQLITE_OK) {
    perror("exec()");
    sqlite3_free(err_message);
    return NULL;
  }else {
	  return data;
  } 
  sqlite3_close(db);
}



//数据库查询接口
char  *SqlQuery(char *sql,QueryData callback) {
  sqlite3 *db;
  int rc;
  char *err_message;
  char *data=NULL;

  data=malloc(sizeof(char)*N);
  memset(data,0,sizeof(N));
  //链接数据库
  rc = sqlite3_open("Database.db", &db);
  if (rc != SQLITE_OK) {
    perror("opendatabase()");
	sqlite3_free(err_message);
    return NULL;
  }
  rc = sqlite3_exec(db, sql,callback, data, &err_message);
  sqlite3_close(db);
  if (rc != SQLITE_OK) {
    perror("exec()");
    sqlite3_free(err_message);
    return NULL;
  }else {
	  return data;
  } 
  sqlite3_close(db);
}



//sql回调函数
int QueryAllData(void *data, int argc, char **argv, char **azColName) {
  char *ret = (char *)data;
  if (argc > 0) {
    // 打印数据
    for (int i = 0; i < argc; i++) {
      strcat(ret, argv[i] ? argv[i] : "NULL");
      strcat(ret, ","); // 添加分隔符
    }
    strcat(ret, "\n"); // 添加换行符;
  }
  return 0;
}



//查询单个user数据并返回值
int QuerySLData(void *data, int argc, char **argv, char **azColName) {
  char *result = (char *)data;
  if (argc > 0) {
    // 打印数据
    for (int i = 0; i < argc; i++) {
      strcat(result, argv[i] ? argv[i] : "NULL");
      strcat(result, ","); // 添加分隔符
    }
  }
  return 0;
}


//添加日志记录模块API
void AddOplog(Client *c, char *optype, char *content, char *user) {
  //-----日志处理
  //获取时间字符串
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  char tempstr[30];
  strftime(tempstr, sizeof(tempstr) - 1, "%Y-%m-%d %H:%M:%S", t);
  char sql[N];
  char *ip = inet_ntoa(c->info.sin_addr);
  int port = ntohs(c->info.sin_port);
  sprintf(sql, "insert into Userlog (op_account,op_type,op_content,op_ip,op_port,op_time) values ('%s','%s','%s','%s',%d,'%s');",user, optype, content, ip, port, tempstr);
  if (Sqlop(sql)) {
    puts("添加日志成功");
  } else {
    puts("添加失败");
  }
  return;
}



//客户端注销
void Userlogout(Client *c){
char sql[N]="";
char sql1[N]="";
char *content = "用户注销";
sprintf(sql1,"update UserInfo set user_isLogin=0 where user_account='%s';",c->User);
Sqlop(sql1);
AddOplog(c, USER_LOGOUT, content,c->User);
return;
}
