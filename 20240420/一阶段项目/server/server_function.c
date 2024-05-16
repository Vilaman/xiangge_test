/***********************************************************************************
# File Name:    server_function.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月14日 星期二 15时29分56秒
 **********************************************************************************/

#include "server_error.h"
#include "server_function.h"
#include "basedata.h"
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

typedef int (*QueryData)(void *, int , char **, char **);
char userinfostr[N]="";
User *u = (User *)malloc(sizeof(User));

//接收客户端操作函数
char *confirm_Op(int client_fd) {
  char data[30];
  int readcount = read(client_fd, data, sizeof(data) - 1);
  if (readcount > 0) {
    data[readcount] = '\0';
    puts(data);
  } else if (readcount = 0) {

    return NULL;
  } else {

    //	return Accepterror_server(ACCEPT_DATA_ERROR);
  }
  if (strcmp(data, CLIENT_LOGIN) == 0) { //登录
    writedata(ALLOW, client_fd);
    return CLIENT_LOGIN;
  } else if (strcmp(data, USER_REGISTER) == 0) { //注册
    writedata(REGISTER_ALLOW, client_fd);
    return USER_REGISTER;
  } else if (strcmp(data, DELETE_USER) == 0) { //删除
    writedata(DELETE_ALLOW, client_fd);
    return DELETE_USER;
  } else if (strcmp(data, CHANGE_INFO) == 0 ||
             strcmp(data, NORMAL_CHANGEINFO) == 0) { //修改
    if (strcmp(data, CHANGE_INFO) == 0) {
      writedata(CHANGE_ALLOW, client);
    } else {
      writedata(NORMALCH_ALLOW, client_info);
    }
    return CHANGE_INFO;
  } else if (strcmp(data, QUERY_INFO) == 0 ||
             strcmp(data, NORMAL_QUERYINFO) == 0) { //查信息
    if (strcmp(data, QUERY_INFO) == 0) {
      writedata(QUERYINFO_ALLOW, client);
    } else {
      writedata(NORMALQI_ALLOW, client_info);
    }
    return QUERY_INFO;
  } else if (strcmp(data, QUERY_OPRATION) == 0 ||
             strcmp(data, NORMAL_QUERYOP) == 0) { //查操作
    if (strcmp(data, CHANGE_INFO) == 0) {
      writedata(QUERYOP_ALLOW, client);
    } else {
      writedata(NORMALQOP_ALLOW, client_info);
    }
    return QUERY_OPRATION;
  }
  return NULL;
}


//返回接受客户端操作传入操作
void writedata(char *optype,int client_fd){

	int writecount;
	if((writecount=write(client_fd,optype,M))<0){
		perror("write()");
		exit(-1);
	}
	puts(optype);
	return;
}

//主操作函数块
void server_Mainop(int client_fd){
	if(strcmp(confirm_Op(client_fd),CLIENT_LOGIN)==0){
		loginMoudle(client_fd);
	}else if(strcmp(confirm_Op(client_fd),USER_REGISTER)==0){
		RegisterMoudle(client_fd);
	}else if(strcmp(confirm_Op(client_fd),DELETE_USER)==0){
		DeleteMoudle(client_fd);
	}else if(strcmp(confirm_Op(client_fd),CHANGE_INFO)==0){
		ChangeMoudle(client_fd);
	}else if(strcmp(confirm_Op(client_fd),QUERY_INFO)==0){
		QueryInfoMoudle(client_fd);
	}else if(strcmp(confirm_Op(client_fd),QUERY_OPRATION)==0){
		QueryOpMoudle(client_fd);
	}else{
		//错误处理
	}
	return;
}

//从数据表中拿到所有用户信息
void getInfo(User *u,char *str){


}


//登录模块
void loginMoudle(Client *c) {
  bool isLogMoudle = true;
  int readcount;
  char acceptdata[60];
  char sql[N];
  char *logaccount;
  char *logpass;
  char *content = "登录系统";
  int count = 0;
  while (isLogMoudle) {
    if ((readcount = read(c->client_fd, acceptdata, sizeof(acceptdata))) > 0) {
      acceptdata[readcount] = '\0';
      logaccount = strtok(acceptdata, ",");
      logpass = strtok(NULL, ",");
    } else {
      return;
    }
    sprintf(sql, "selete * from UserInfo where user_account='%s'", logaccount);
    int writecount;
    if ((char *backdata = SqlQuery(sql, QuerySLData)) != NULL) {
      memset(sql, 0, sizeof(sql));
      sprintf(sql,
              "selete * from UserInfo where user_account='%s' and "
              "user_password='%s'",
              logaccount, logpass);
      if ((char *backdata = SqlQuery(sql, QuerySLData)) != NULL) {
        memset(sql, 0, sizeof(sql));
		count++;
        sprintf(sql,
                "update UserInfo set user_isLogin=1 where user_account='%s'",
                logaccount);
        Sqlop(sql);
        AddOplog(c, USER_LOGIN, content, logaccount);
        if (strcmp(logaccount, "admin") == 0) {
          if ((writecount = write(c->client_fd, ADMIN_LOGIN, M)) > 0) {
            isLogMoudle = false;
          } else {
            //客户端下线
          }
        } else {
          if ((writecount = write(c->client_fd, NORMALUSER_LOGIN, M)) > 0) {
            isLogMoudle = false;
          } else {
            //客户端下线
          }
        }
      }else{
		  if ((writecount = write(c->client_fd,"LOGIN_PASSWORD_ERROR" , M)) > 0) {
		
          } else{}
	  }
    }else{
		
		  if ((writecount = write(c->client_fd,"LOGIN_USERNAME_ERROR" , M)) > 0) {
		
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
	char data[100];
	char sql[N];
	char *content="注册新用户";
	char *newaccount;
	char *newpass;
	char *newname;

	if((readcount=read(c->client_fd,data,sizeof(data)-1))>0){
		data[readcount]='\0';
	}else{
		return;
	}
	newaccount=strtok(data,",");
	newpass=strtok(NULL,",");
	newname=strtok(NULL,",");
	sprintf(sql,"insert into UserInfo (user_account,user_password,user_name) values ('%s','%s','%s')");
	if(Sqlop(sql)){
		if((writecount=write(c->client_fd,REGISTER_SUCCESS,M))>0){
			AddOplog(c,USER_REGISTER,content,"admin");
		}else{
			//客户端下线
		}
	}else{
		if((writecount=write(c->client_fd,REGISTER_EXIST_ERROR,M))<0){
			//客户端下线
		}
	}
	return;
}



//删除模块
void DeleteMoudle(Client *c) {
  int readcount;
  char acceptdata[D];
  char sql[N];
  char *content = "删除用户";

  if ((readcount = read(c->client_fd, acceptdata, M - 1)) > 0) {
    acceptdata[readcount] = '\0';
  } else {
    //
    return;
  }
  sprintf(sql, "delete from UserInfo where user_account='%s'", acceptdata);
  int writecount;
  if (Sqlop(sql)) {
    if ((writecount = write(c->client_fd, DELETE_SUCCESS, M)) < 0) {
      //客户端下线或未发送成功
    } else {
      AddOplog(c, DELETE_USER, content,account);
    }
  } else {
    if ((writecount = write(c->client_fd, "DELETE_NOTEXIST_ERROR", M)) < 0) {

      //客户端下线或者未发送出
      return;
    }
  }
  return;
}


//修改信息模块
void ChangeMoudle(Client *c) {
  User u1; //定义一个结构体接收
  char data[250];
  int readcount;
  int writecount;
  char sql[N];
  char *content = "删除用户";
  if((readcount=read(c->client_fd.data.sizeof(data)-1))>0){
	  data[readcount]='\0';
  }else{

	  //错误处理
	  return;
  }
  u1.account=strtok(data,",");
  u1.name=strtok(NULL,",");
  u1.pass=strtok(NULL,",");
  u1.sex=strtok(NULL,",");
  u1.age=int(*(strtok(NULL,","))-'0');
  u1.address=strtok(NULL,",");
  sprintf(sql,"update UserInfo set user_name='%s',user_pass='%s',user_sex='%s',user_age=%d,user_address='%s' where user_account='%s'",u1.name,u1.pass,u1.sex,u1.age,u1.address,u1.account);
  if (Sqlop(sql)) {
    if ((writecount = write(c->client_fd, CHANGE_SUCCESS, M)) < 0) {
      //客户端下线或未发送成功
    } else {
      AddOplog(c, CHANGE_INFO, content,u1.account);
    }
  } else {
    if ((writecount = write(c->client_fd, "CHANGE_NOTEXIST_ERROR", M)) < 0) {

      //客户端下线或者未发送出
      return;
    }
  }

}





//查看信息模块
void QueryInfoMoudle(int client_fd) {
  char data[MAXSIZE];
  int readcount;
  int writecount;
  char recvdata[D];
  char sql[N];
  char *content = "查询信息";
  if ((readcount = read(c->client_fd.data.sizeof(data) - 1)) > 0) {
    recvdata[readcount] = '\0';
  } else {

    //错误处理
    return;
  }
  sprintf(sql, "select * from UserInfo where user_account='%s'", recvdata);
  strcpy(data, SqlQuery(sql, QueryAllData));
  if (strcmp(data, "") != 0) {
    if ((writecount = write(c->client_fd, data, sizeof(data))) < 0) {
      //客户端下线或未发送成功
    } else {
      AddOplog(c, QUERYINFO, content, recvdata);
    }
  } else {
    if ((writecount = write(c->client_fd, "QUERYINFO_FAIL", M)) < 0) {

      //客户端下线或者未发送出
      return;
    }
  }
}






//查看历史操作模块
void QueryOpMoudle(int client_fd) {
  char data[250];
  int readcount;
  int writecount;
  char recvdata[D];
  char sql[N];
  char *content = "查询操作历史";
  if ((readcount = read(c->client_fd.data.sizeof(data) - 1)) > 0) {
    recvdata[readcount] = '\0';
  } else {

    //错误处理
    return;
  }
  sprintf(sql, "select * from Userlog where op_account='%s'", recvdata);
  strcpy(data, SqlQuery(sql, QueryAllData));
  if (strcmp(data, "") != 0) {
    if ((writecount = write(c->client_fd, data, sizeof(data))) < 0) {
      //客户端下线或未发送成功
    } else {
      AddOplog(c, QUERY_LOG, content, recvdata);
    }
  } else {
    if ((writecount = write(c->client_fd, "QUERYOP_FAIL", M)) < 0) {

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



//数据库查询接口
char  *SqlQuery(char *sql,QueryData callback) {
  sqlite3 *db;
  int rc;
  char *err_message;
  char data[N]="";
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
      strcat(ret, " "); // 添加分隔符
      printf("%s ", argv[i] ? argv[i] : "NULL");
    }
    strcat(ret, "\n"); // 添加换行符;
  }
  return 0;
}



//查询单个user数据并返回值
int QuerySLData(void *data, int argc, char **argv, char **azColName) {
  char *result = (char *)data;
  if (argc > 0) {
    for (int i = 0; i < argc; i++) {
      printf("%s ", azColName[i]); // 打印列名
    }
    printf("\n");
    // 打印数据
    for (int i = 0; i < argc; i++) {
      strcat(result, argv[i] ? argv[i] : "NULL");
      strcat(result, ","); // 添加分隔符
      printf("%s ", argv[i] ? argv[i] : "NULL");
    }
    printf("\n"); // 添加换行符;
  }
  return 0;
}


//添加日志记录模块API
void AddOplog(Client *c, char *optype, char *content,char *user) {
  //-----日志处理
  //获取时间字符串
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  char tempstr[30];
  strftime(tempstr, sizeof(tempstr) - 1, "%Y-%m-%d %H:%M:%S", t);
  char sql[N];
  char *ip = inet_ntoa(client.sin_addr);
  int port = ntohs(client.sin_port);
  sprintf(sql,
          "insert into Userlog "
          "(op_account,op_type,op_content,op_ip,op_port,op_time) values "
          "('%s','%s','%s','%s',%d,'%s')",
        user, optype, content, ip, port, tempstr);
  if (Sqlop(sql)) {
    puts("添加日志成功");
  } else {
    puts("添加失败");
  }
  return;
}
