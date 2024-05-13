#ifndef _CONNETCTION_POOL_
#define _CONNETCTION_POOL_

#include <stdio.h>
#include <list>
#include <mysql/mysql.h>
#include <error.h>
#include <string.h>
#include <iostream>
#include <string>
#include "locker.h"
#include "log.h"

// 数据库连接池
class connetion_pool{
public:
    MYSQL *Getconnection();                 //获取数据库连接
    bool releaseConnection(MYSQL *conn);    //释放连接
    int GetFreeConn();                      //获取连接
    void DestroyPool();                     //销毁所有连接
    //单例模式
    static connetion_pool *Getconnection();
    //初始化
    void init(std::string url,std::string User, std::string PassWord, std::string DataBaseName, int Port, int MaxConn);
private:
    connetion_pool();
    ~connetion_pool();
    int m_MaxConn;              //最大连接数
    int m_CurConn;              //当前已使用的连接数
    int m_FreeConn;             //当前空闲的连接数
    locker lock;
    std::list<MYSQL*>connList;  //连接池
    sem reserve;
private:
    std::string m_url;          //主机地址
    int m_Port;                 //数据库端口号
    std::string m_User;         //登陆数据库用户名
    std::string m_Password;     //登陆数据库密码
    std::string m_DatabassName; //使用数据库名
};
// 资源获取即初始化
class connetionRAII{
public:
    connetionRAII(MYSQL **con,connetion_pool *connpoll);
    ~connetionRAII();
private:
    MYSQL *conRAII;
    connetion_pool *poolRAII;
}

#endif