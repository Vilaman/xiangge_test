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
			LOG_ERROR("MYSQL Error");
			exit(1);
		}
		con = mysql_real_connect(con, url.c_str(), User.c_str(), PassWord.c_str(), DataBaseName.c_str(), Port, NULL, 0);
		if (con == NULL) {
			LOG_ERROR("MYSQL Error");
			exit(1);
		}
		connList.push_back(con);
		++m_FreeConn;
	}
	reserve = sem(m_FreeConn);
	m_MaxConn = m_FreeConn;
}

MYSQL* connection_pool::GetConnection() {
	MYSQL* con = NULL;

	if (0 == connList.size())
		return NULL;

	reserve.wait();

	lock.lock();

	con = connList.front();
	connList.pop_front();

	--m_FreeConn;
	++m_CurConn;

	lock.unlock();
	return con;
}

bool connection_pool::releaseConnection(MYSQL *con) {
	if (con == NULL) {
		return false;
	}
	lock.lock();

	connList.push_back(con);
	++m_FreeConn;
	--m_CurConn;

	lock.unlock();
	reserve.post();
	return true;
}

int connection_pool::GetFreeConn() {
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
}

connectionRAII::connectionRAII(MYSQL** con, connection_pool* connPool) {
	*con = connPool->GetConnection();
	conRAII = *con;
	poolRAII = connPool;
}

connectionRAII::~connectionRAII() {
	poolRAII->releaseConnection(conRAII);
}
