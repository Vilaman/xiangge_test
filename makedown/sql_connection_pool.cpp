#include <stdio.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <iostream>
#include <pthread.h>
#include <list>
#include "sql_connection_pool.h"

connection_pool::connection_pool() {
	m_CurConn = 0;
	m_FreeConn = 0;
}

connection_pool::~connection_pool() {
	DestroyPool();
}

connection_pool* connection_pool::GetInstance() {
	static connection_pool connPool;
	return &connPool;
}

void connection_pool::init(std::string url, std::string User, std::string PassWord, std::string DataBaseName, int Port, int MaxConn) {
	m_url = url;
	m_Port = Port;
	m_User = User;
	m_Password = PassWord;
	m_DatabassName = DataBaseName;
	for (int i = 0; i < MaxConn; i++) {
		MYSQL* con = NULL;
		con = mysql_init(con);
		if (con == NULL) {
			LOG_ERROR("MYSQL Error mysql_init");
			exit(1);
		}
		con = mysql_real_connect(con, url.c_str(), User.c_str(), PassWord.c_str(), DataBaseName.c_str(), Port, NULL, 0);
		if (con == NULL) {
			LOG_ERROR("MYSQL Error mysql_real_connect");
			exit(1);
		}
		connList.push_back(con);
		++m_FreeConn;
	}
	reserve = sem(m_FreeConn);
	m_MaxConn = m_FreeConn;
	LOG_INFO("connection_pool init success");
}

MYSQL* connection_pool::GetConnection() {
	MYSQL* con = NULL;
	if (0 == connList.size()){
		return NULL;
	}
	reserve.wait();
	lock.lock();
	con = connList.front();
	connList.pop_front();
	--m_FreeConn;
	++m_CurConn;
	lock.unlock();
	LOG_INFO("connection_pool GetConnection success");
	return con;
}

bool connection_pool::releaseConnection(MYSQL *con) {
	if (con == NULL) {
		LOG_ERROR("connection_pool releaseConnection Error");
		return false;
	}
	lock.lock();

	connList.push_back(con);
	++m_FreeConn;
	--m_CurConn;

	lock.unlock();
	reserve.post();
	LOG_INFO("connection_pool releaseConnection success");
	return true;
}

int connection_pool::GetFreeConn() {
	LOG_INFO("connection_pool GetFreeConn success");
	return this->m_FreeConn;
}

void connection_pool::DestroyPool() {
	lock.lock();
	if (connList.size() > 0) {
		std::list<MYSQL*>::iterator it;
		for (it = connList.begin(); it != connList.end(); it++) {
			MYSQL* con = *it;
			mysql_close(con);
		}
		m_CurConn = 0;
		m_FreeConn = 0;
		connList.clear();
	}
	lock.unlock();
	LOG_INFO("connection_pool DestroyPool success");
}

connectionRAII::connectionRAII(MYSQL** con, connection_pool* connPool) {
	*con = connPool->GetConnection();
	conRAII = *con;
	poolRAII = connPool;
}

connectionRAII::~connectionRAII() {
	poolRAII->releaseConnection(conRAII);
}


/* test main
int main(){
    std::string m_user = "root";            //登陆数据库用户名
    std::string m_passWord = "root";        //登陆数据库密码
    std::string m_databaseName = "yourdb";  //使用数据库名
    int m_sql_num = 8;
    
    connection_pool *m_connPool = connection_pool::GetInstance();
    m_connPool->init("localhost", m_user, m_passWord, m_databaseName, 3306, m_sql_num);

	m_connPool->GetFreeConn();
    m_connPool->GetConnection();

    return 0;
}
*/