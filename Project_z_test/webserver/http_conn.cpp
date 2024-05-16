#include "http_conn.h"

#include <mysql/mysql.h>
#include <fstream>

std::map<std::string, std::string> users;
//*************************************************************************************
void http_conn::initmysql_result(connection_pool* connPool) {
	//先从连接池中取一个连接
	MYSQL* mysql = NULL;
	connectionRAII mysqlcon(&mysql, connPool);

	//在user表中检索username，passwd数据，浏览器端输入
	if (mysql_query(mysql, "SELECT username,passwd FROM user")) {
		LOG_ERROR("SELECT error:%s\n", mysql_error(mysql));
	}

	//从表中检索完整的结果集
	MYSQL_RES* result = mysql_store_result(mysql);

	//返回结果集中的列数
	int num_fields = mysql_num_fields(result);

	//返回所有字段结构的数组
	MYSQL_FIELD* fields = mysql_fetch_fields(result);

	//从结果集中获取下一行，将对应的用户名和密码，存入map中
	while (MYSQL_ROW row = mysql_fetch_row(result)) {
		std::string temp1(row[0]);
		std::string temp2(row[1]);
		users[temp1] = temp2;
	}
}

//对文件描述符设置非阻塞
int setnonblocking(int fd) {
	int old_option = fcntl(fd, F_GETFL);
	int new_option = old_option | O_NONBLOCK;
	fcntl(fd, F_SETFL, new_option);
	return old_option;
}

//将内核事件表注册读事件，ET模式，选择开启EPOLLONESHOT
void addfd(int epollfd, int fd, bool one_shot, int TRIGMode) {
	epoll_event event;
	event.data.fd = fd;

	if (1 == TRIGMode)
		event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
	else
		event.events = EPOLLIN | EPOLLRDHUP;

	if (one_shot)
		event.events |= EPOLLONESHOT;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
	setnonblocking(fd);
}

//从内核时间表删除描述符
void removefd(int epollfd, int fd) {
	epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, 0);
	close(fd);
}

//将事件重置为EPOLLONESHOT
void modfd(int epollfd, int fd, int ev, int TRIGMode) {
	epoll_event event;
	event.data.fd = fd;

	if (1 == TRIGMode)
		event.events = ev | EPOLLET | EPOLLONESHOT | EPOLLRDHUP;
	else
		event.events = ev | EPOLLONESHOT | EPOLLRDHUP;

	epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);
}

int http_conn::m_user_count = 0;
int http_conn::m_epollfd = -1;

//关闭连接，关闭一个连接，客户总量减一
void http_conn::close_conn(bool real_close) {
	if (real_close && (m_sockfd != -1)) {
		printf("close %d\n", m_sockfd);
		removefd(m_epollfd, m_sockfd);
		m_sockfd = -1;
		m_user_count--;
	}
}

//初始化连接,外部调用初始化套接字地址
void http_conn::init(int sockfd, const sockaddr_in& addr, char* root, int TRIGMode, std::string user, std::string passwd, std::string sqlname)
{
	m_sockfd = sockfd;
	m_address = addr;

	addfd(m_epollfd, sockfd, true, m_TRIGMode);
	m_user_count++;

	//当浏览器出现连接重置时，可能是网站根目录出错或http响应格式出错或者访问的文件中内容完全为空
	doc_root = root;
	m_TRIGMode = TRIGMode;

	strcpy(sql_user, user.c_str());
	strcpy(sql_passwd, passwd.c_str());
	strcpy(sql_name, sqlname.c_str());

	init();
}

//初始化新接受的连接
//check_state默认为分析请求行状态
void http_conn::init()
{
	mysql = NULL;
	bytes_to_send = 0;
	bytes_have_send = 0;
	m_linger = false;
	m_read_idx = 0;
	m_write_idx = 0;
	timer_flag = 0;
	improv = 0;
	m_state = 0;

	memset(m_read_buf, '\0', READ_BUFFER_SIZE);
	memset(m_write_buf, '\0', WRITE_BUFFER_SIZE);
	memset(m_real_file, '\0', FILENAME_LEN);
}


//循环读取客户数据，直到无数据可读或对方关闭连接
//非阻塞ET工作模式下，需要一次性将数据读完
bool http_conn::read_once()
{
	if (m_read_idx >= READ_BUFFER_SIZE)
	{
		return false;
	}
	int bytes_read = 0;

	//LT读取数据
	if (0 == m_TRIGMode)
	{
		bytes_read = recv(m_sockfd, m_read_buf + m_read_idx, READ_BUFFER_SIZE - m_read_idx, 0);
		m_read_idx += bytes_read;

		if (bytes_read <= 0)
		{
			return false;
		}

		return true;
	}
	//ET读数据
	else
	{
		while (true)
		{
			bytes_read = recv(m_sockfd, m_read_buf + m_read_idx, READ_BUFFER_SIZE - m_read_idx, 0);
			if (bytes_read == -1)
			{
				if (errno == EAGAIN || errno == EWOULDBLOCK)
					break;
				return false;
			}
			else if (bytes_read == 0)
			{
				return false;
			}
			m_read_idx += bytes_read;
		}
		return true;
	}
}

void http_conn::unmap()
{
	if (m_file_address)
	{
		munmap(m_file_address, m_file_stat.st_size);
		m_file_address = 0;
	}
}
bool http_conn::write()
{
	int temp = 0;

	if (bytes_to_send == 0)
	{
		modfd(m_epollfd, m_sockfd, EPOLLIN, m_TRIGMode);
		init();
		return true;
	}

	while (1)
	{
		temp = writev(m_sockfd, m_iv, m_iv_count);

		if (temp < 0)
		{
			if (errno == EAGAIN)
			{
				modfd(m_epollfd, m_sockfd, EPOLLOUT, m_TRIGMode);
				return true;
			}
			unmap();
			return false;
		}

		bytes_have_send += temp;
		bytes_to_send -= temp;
		if (bytes_have_send >= m_iv[0].iov_len)
		{
			m_iv[0].iov_len = 0;
			m_iv[1].iov_base = m_file_address + (bytes_have_send - m_write_idx);
			m_iv[1].iov_len = bytes_to_send;
		}
		else
		{
			m_iv[0].iov_base = m_write_buf + bytes_have_send;
			m_iv[0].iov_len = m_iv[0].iov_len - bytes_have_send;
		}

		if (bytes_to_send <= 0)
		{
			unmap();
			modfd(m_epollfd, m_sockfd, EPOLLIN, m_TRIGMode);

			if (m_linger)
			{
				init();
				return true;
			}
			else
			{
				return false;
			}
		}
	}
}

void http_conn::process() {
	LOG_INFO("process");
    /*
    这里进行http解析等操作
    */
}
