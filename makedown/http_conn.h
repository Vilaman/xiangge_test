#ifndef HTTPCONNECTION_H
#define HTTPCONNECTION_H

#include <stdio.h>
#include <string>
#include <map>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <mysql/mysql.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/uio.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#include "locker.h"
#include "sql_connection_pool.h"
#include "log.h"
#include "lst_timer.h"

class http_conn {
public:
	// 文件名称长度、缓冲区大小和HTTP方法的常量
	static const int FILENAME_LEN = 200;
	static const int READ_BUFFER_SIZE = 2048;
	static const int WRITE_BUFFER_SIZE = 1024;

public:
	http_conn(){}
	~http_conn(){}

public:
	void init(int sockfd, const sockaddr_in& addr, char*, int, std::string user, std::string passwd, std::string sqlname);
	void close_conn(bool real_close = true);
	void process();
	bool read_once();
	bool write();
	sockaddr_in* get_address() {
		return &m_address;
	}
	void initmysql_result(connection_pool* connPool);
	// 计时器和标志的公有变量
	int timer_flag;
	int improv;

private:
	// 用于内部处理的私有方法声明
	void init();
	void unmap();

public:
	// 类级别的静态变量
	static int m_epollfd;		// Epoll文件描述符
	static int m_user_count;	// 用户计数
	MYSQL* mysql;
	int m_state;  //读为0, 写为1

private:
	// 用于处理连接、缓冲区、状态等的私有变量
	int m_sockfd;							// 套接字文件描述符
	sockaddr_in m_address;					// 客户端地址
	char m_read_buf[READ_BUFFER_SIZE];		// 读取数据的缓冲区
	int m_read_idx;
	char m_write_buf[WRITE_BUFFER_SIZE];
	int m_write_idx;
	char m_real_file[FILENAME_LEN];
	bool m_linger;
	char* m_file_address;
	struct stat m_file_stat;
	struct iovec m_iv[2];
	int m_iv_count;
	int bytes_to_send;
	int bytes_have_send;
	char* doc_root;

	std::map<std::string, std::string> m_users;	// 用户凭证的映射
	int m_TRIGMode;	// 触发模式

	char sql_user[100];		// 数据库用户名
	char sql_passwd[100];	// 数据库密码
	char sql_name[100];		// 数据库名称
};



#endif // !HTTPCONNECTION_H
