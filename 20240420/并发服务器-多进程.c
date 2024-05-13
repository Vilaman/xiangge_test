/***********************************************************************************
# File Name:    110-并发服务器.c
# Author:       dengiswen
# Mail:         dengiswen@gmail.com
# Created Time: 2024年05月07日 星期二 16时30分44秒
 **********************************************************************************/

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <wait.h>
#include<sys/msg.h>

#define NUM 10 //最大群聊数量
void *handleClient(void *);
void *sendAll(void *);
void saveLog(char *);


typedef struct Client {
  struct sockaddr_in info; //客户端信息
  int fd;                  //套接字
} Client;


typedef struct message{

	long type;
	char data[101];
	int port;
}message;

Client *clients; //数组



pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;




int main() {
  pthread_t pid; //保存每一次的线程号
  int client_fd; //保存每一次连接的客户端套接字
  //初始化客户端数组并将客户端fd初始为-1
  
	

  clients = malloc(sizeof(Client) * NUM);
  for (int i = 0; i < NUM; i++) {
    clients[i].fd = -1;
  }

  //创建线程来处理群发
  pthread_create(&pid, NULL, sendAll, NULL);

  // 1 创建套接字
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    perror("服务器套接字失败");
    exit(0);
  }
  struct sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_port = htons(8088);
  server.sin_addr.s_addr = INADDR_ANY; //接受任意ip地址的连接

  // 2.绑定端口
  if (bind(server_fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
    perror("bind端口失败");
    exit(0);
  }
  // 3. 监听
  if (listen(server_fd, 10) < 0) {
    perror("监听失败");
    exit(0);
  }
  // 4. 接受连接
  struct sockaddr_in client;
  socklen_t len = sizeof(client);
  puts("等待客户端的连接");
  while ((client_fd = accept(server_fd, (struct sockaddr *)&client, &len)) >
         0) {
    //已连接
    //处理客户端连接
    //保证每个客户端连接保存套接字的是一个新的变量和空间，彼此线程互不影响
    // int *fd = malloc(sizeof(int));
    Client *c = malloc(sizeof(Client));
    c->fd = client_fd;
    c->info = client;
    //保存到客户段数组中
    for (int i = 0; i < NUM; i++) {
      if (clients[i].fd == -1) {
        clients[i] = *c;
        break;
      }
    }

    //输出客户端已连接

    char *ip = inet_ntoa(client.sin_addr);
    int port = ntohs(client.sin_port);
    printf("已接受客户端:%s:%d的连接\n", ip, port);
    //等待下一位客户端连接
    puts("等待下一位客户端的连接");

    //用子进程处理客户端的连接
    int pid = fork();
	message *m=malloc(sizeof(message));
    if (pid > 0) {
      //父进程
      // close(client_fd);
      // wait(NULL);
	  
    } else if (pid == 0) {
      //负责处理客户端连接
      handleClient(c);
      exit(0);
    }
  }
  // 5. 关闭
  close(server_fd);
  pthread_join(pid, NULL);
  return 0;
}



//群聊
void *sendAll(void *args) {

  key_t key = ftok("fifotest", 34);
  int msqid = msgget(key, IPC_CREAT | 0666);
  message m;
  m.type = 34;
  //接送群消息
  char data[200] = "";
  int readcount;
  while (1) {
    readcount = msgrcv(msqid, &m, sizeof(message), m.type, 0);
    if (readcount > 0) {
      snprintf(data, sizeof(data) - 1, "用户%d说：%s", m.port, m.data);

      for (int i = 0; i < NUM; i++) {
        if (clients[i].fd != -1) {
          write(clients[i].fd, data, strlen(data));
        } else {
          break;
        }
      }
    }
  }
}




//传递的就是指向客户端套接字的指针
void *handleClient(void *args) {
  //分离
  pthread_t pid = pthread_self();
  pthread_detach(pid); //设置为分离线程
  Client *c = (Client *)args;
  printf("线程处理客户端:%d\n", c->fd);
  //保存日志记录
  //上锁
  pthread_mutex_lock(&mutex);
  saveLog(c);
  //解锁
  pthread_mutex_unlock(&mutex);
  //-----
  //处理数据
	  int msqid;
	  key_t key;
	  if((key=ftok("fifotest",34))==-1){
		perror("ftok");
		exit(1);
	  }
	 msqid= msgget(key,IPC_CREAT |0666);
	 message m;
  int readCount;
  char data[101] = "";
  while ((readCount = read(c->fd, data, sizeof(data) - 1)) > 0) {
    data[readCount] = '\0';
    printf("服务器接受数据:%s\n", data);


//处理消息队列

	m.type=34;
	m.port=port;
	strcpy(m.data,data);

	msgsnd(msqid,&m,sizeof(message),0);
	memset(&m,0,sizeof(message));
	/*
	 *
	 *将数据发送给群发线程
	 *
	 * */
  }
  //退出while 客户端关闭了
  puts("客户端下线");
  free(args); //释放保存客户端套接字的内存，因为是malloc分配的
  // pthread_exit(NULL);
  return NULL;
}


/*void saveLog(char *str) {
  // 1.打开文件，追加模式
  FILE *fp = fopen("clientlog.txt", "a");
  if (fp == NULL) {
    perror("文件打卡失败");
    return;
  }
  // 2 追加
  fputs(str, fp);
  // 3关闭
  fclose(fp);
  puts("写入日志成功");
}*/


void addlogwithsql(Client *c) {
  char *ip = inet_ntoa(c->info.sin_addr);
  int port = ntohs(c->info.sin_port);
  //-----日志处理
  //生成要保存的日志字符串
  char timestr[100] = "";
  //获取时间字符串
  char *type = "LOGIN";
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  char tempstr[30];
  strftime(tempstr, sizeof(tempstr) - 1, "%Y-%m-%d %H:%M:%S", t);
  sqlite3 *db;
  int rc;
  char *err_meagger;
  rc=sqlite3_open("./test.db",&db);
  if(rc!=SQLITE_OK){
	perror("open()");
	return;
  }
  char sql[200];






}
