#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <list>                  // 用于存放任务队列
#include <stdio.h>
#include <stdexcept>             // 用于抛出异常
#include <pthread.h>             // 提供多线程功能
#include "locker.h"              // 提供互斥锁、信号量等同步机制
#include "sql_connection_pool.h" // 提供数据库连接池功能
#include "http_conn.h"           // 处理http连接的类

// 线程池类定义，T为任务类类型
template<typename T>
class threadpool {
public:
    // 构造函数：需要传入模型切换参数、数据库连接池、线程数量和最大请求数
    threadpool(int actor_model, connection_pool* connPool, int thread_number = 8, int max_requests = 10000);
    ~threadpool();  // 析构函数

    // 两种添加任务的方法：
    bool addend(T* request, int state); // 添加任务并设置状态
    bool addend(T* request);            // 添加任务，默认状态

private:
    static void* worker(void* arg); // 线程池中的每个线程执行这个函数
    void run();                     // 工作线程从队列中取出并执行任务

    int m_thread_number;            // 线程池中的线程数量
    int m_max_requests;             // 请求队列中最多允许的请求数
    pthread_t* m_threads;           // 线程数组
    std::list<T*> m_workqueue;      // 请求队列
    locker m_queuelocker;           // 请求队列的互斥锁
    sem m_queuestat;                // 信号量，用于表示是否有任务需要处理
    connection_pool* m_connPool;    // 数据库连接池
    int m_actor_model;              // 用于模型切换，决定处理请求的方式
};

// 构造函数实现：初始化线程池，创建指定数量的线程
template<typename T>
threadpool<T>::threadpool(int actor_model, connection_pool* connPool, int thread_number, int max_requests)
    :m_actor_model(actor_model), m_thread_number(thread_number), m_max_requests(max_requests), m_threads(NULL), m_connPool(connPool) {
    if (thread_number <= 0 || max_requests <= 0) {
        throw std::exception(); // 如果线程数量或最大请求数不合理，抛出异常
    }
    m_threads = new pthread_t[m_thread_number];
    if (!m_threads) {
        throw std::exception(); // 如果线程数组创建失败，抛出异常
    }
    for (int i = 0; i < thread_number; i++) {
        // 为每个线程创建线程，并将其脱离，使线程独立于创建它的线程
        if (pthread_create(m_threads + i, NULL, worker, this) != 0) {
            delete[] m_threads;
            throw std::exception(); // 线程创建失败，抛出异常
        }
        if (pthread_detach(m_threads[i])) {
            delete[] m_threads;
            throw std::exception(); // 线程脱离失败，抛出异常
        }
    }
}

template<typename T>
threadpool<T>::~threadpool() {
    delete[] m_threads; // 析构函数，释放线程数组
}

template<typename T>
bool threadpool<T>::addend(T* request, int state) {
    m_queuelocker.lock(); // 上锁，保护请求队列
    if (m_workqueue.size() >= m_max_requests) {
        m_queuelocker.unlock(); // 如果请求队列已满，解锁并返回false
        LOG_ERROR("addend m_workqueue.size() >= m_max_requests Error");
        return false;
    }
    request->m_state = state;
    m_workqueue.push_back(request); // 将请求添加到队列
    m_queuelocker.unlock();         // 解锁
    m_queuestat.post();             // 通知工作线程有新任务
    return true;
}

template<typename T>
bool threadpool<T>::addend(T* request) {
    // 根据加入请求的重载函数实现，差异在于是否设置状态
    m_queuelocker.lock();
    if (m_workqueue.size() >= m_max_requests)
    {
        m_queuelocker.unlock();
        LOG_ERROR("addend m_workqueue.size() >= m_max_requests Error");
        return false;
    }
    m_workqueue.push_back(request);
    m_queuelocker.unlock();
    m_queuestat.post();
    return true;
}


template<typename T>
void* threadpool<T>::worker(void* arg) {
    // 工作线程的静态成员函数，获取线程池实例并调用运行函数
    threadpool* pool = (threadpool*)arg;
    pool->run();
    return pool;
}

template<typename T>
void threadpool<T>::run() {
    // 工作线程不断从队列中取任务执行
    while (true) {
        m_queuestat.wait();      // 等待任务
        m_queuelocker.lock();    // 取任务时需要上锁
        if (m_workqueue.empty()) {
            m_queuelocker.unlock(); // 如果队列空，则解锁并继续等待
            continue;
        }
        T* request = m_workqueue.front(); // 获取队列的第一个任务
        m_workqueue.pop_front();          // 将这个任务从队列中移除
        m_queuelocker.unlock();           // 解锁
        if (!request) continue;           // 如果任务为空，继续下一次循环

        // 根据模型切换参数处理请求
        if (1 == m_actor_model) {
            // 处理读写事件
            if (0 == request->m_state) {
                if (request->read_once()) {
                    request->improv = 1;
                    connectionRAII mysqlcon(&request->mysql, m_connPool);
                    request->process();
                }
                else {
                    request->improv = 1;
                    request->timer_flag = 1;
                }
            }
            else {
                if (request->write()) {
                    request->improv = 1;
                }
                else {
                    request->improv = 1;
                    request->timer_flag = 1;
                }
            }
        }
        else {
            // 处理HTTP请求
            connectionRAII mysqlcon(&request->mysql, m_connPool); // 使用数据库连接
            request->process(); // 处理请求
        }
    }
}
#endif // !THREADPOOL_H