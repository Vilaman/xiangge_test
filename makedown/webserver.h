#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <cassert>
#include <errno.h>
#include <sys/epoll.h> // Linux下的高效文件事件监测机制
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "threadpool.h" // 线程池头文件，用于处理任务
#include "http_conn.h"  // HTTP连接的处理
#include "log.h"        // 日志记录

// 最大文件描述符数量
const int MAX_FD = 65536;
// 最大事件数，用于epoll
const int MAX_EVENT_NUMBER = 10000;
// 最小超时单位，定时相关
const int TIMESLOT = 5;

class WebServer {
public:
    // 构造函数和析构函数
    WebServer();
    ~WebServer();

    /**
     * 初始化WebServer
     *
     * @param port 端口号
     * @param user 数据库用户名
     * @param passWord 数据库密码
     * @param databaseName 数据库名
     * @param log_write 日志写入模式
     * @param opt_linger socket选项设置
     * @param trigmode 触发模式（水平或边缘触发）
     * @param sql_num 数据库连接数量
     * @param thread_num 线程池内线程数量
     * @param actor_model 采用的模型（比如Reactor或Proactor）
     */
    void init(int port, std::string user, std::string passWord, std::string databaseName,
        int log_write, int opt_linger, int trigmode, int sql_num,
        int thread_num, int actor_model);

    // 初始化线程池
    void thread_pool();
    // 初始化数据库连接池
    void sql_pool();
    // 设置IO多路复用EPOLL的触发模式
    void trig_mode();
    // 监听socket，准备接收连接
    void eventListen();
    // 事件循环，处理事件
    void eventLoop();

    // 处理新的连接建立的定时器设置
    void timer(int connfd, struct sockaddr_in client_address);
    // 调整定时器，用于连接持续活跃的情况
    void adjust_timer(util_timer* timer);
    // 处理定时器超时，断开连接
    void deal_timer(util_timer* timer, int sockfd);
    // 处理客户端数据
    bool dealclinetdata();
    // 处理信号，比如优雅地关闭服务器
    bool dealwithsignal(bool& timeout, bool& stop_server);
    // 处理读事件
    void dealwithread(int sockfd);
    // 处理写事件
    void dealwithwrite(int sockfd);

public:
    // 基础属性
    int m_port;           // 监听的端口号
    char* m_root;         // 服务器根目录
    int m_log_write;      // 日志写入模式（异步/同步）
    int m_actormodel;     // 模型选择

    int m_pipefd[2];      // 管道文件描述符，用于信号处理与主线程通信
    int m_epollfd;        // epoll实例的文件描述符
    http_conn* users;     // 用户连接数组

    // 数据库连接池相关
    connection_pool* m_connPool;
    std::string m_user;
    std::string m_passWord;
    std::string m_databaseName;
    int m_sql_num;                  // 数据库连接数

    // 线程池相关
    threadpool<http_conn>* m_pool;  // 线程池
    int m_thread_num;               // 线程数

    // epoll事件数组
    epoll_event events[MAX_EVENT_NUMBER];

    // 监听文件描述符
    int m_listenfd;
    // socket选项设置
    int m_OPT_LINGER;
    // 触发模式（监听和连接）
    int m_TRIGMode;
    int m_LISTENTrigmode;
    int m_CONNTrigmode;

    // 定时器相关
    client_data* users_timer; // 定时器信息数组
    Utils utils;              // 工具类，通常用于辅助函数
};
#endif