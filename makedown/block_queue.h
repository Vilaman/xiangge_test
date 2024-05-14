#ifndef BLOCK_QUEUE_H
#define BLOCK_QUEUE_H

#include <iostream>
#include <stdlib.h>
#include <pthread.h>    // pthread库，用于多线程编程
#include <sys/time.h>   // 提供获取系统时间的功能
#include "locker.h"     // 互斥锁和条件变量的实现

// block_queue类模板定义，支持存储任意类型的元素
template <class T>
class block_queue {
public:
    // 构造函数，初始化队列大小等属性
    block_queue(int max_size = 1000){
        if(max_size <= 0){
            exit(-1);
        }
        m_max_size = max_size;
        m_array = new T[max_size];  // 动态分配存储空间
        m_size = 0;
        m_frout = -1;   // 前端索引
        m_back = -1;    // 后端索引
    }
    // 清空队列
    void clear(){
        m_mutex.lock();
        m_size = 0;
        m_frout = -1;
        m_back = -1;
        m_mutex.unlock();
    }
    // 析构函数，释放分配的空间
    ~block_queue(){
        m_mutex.lock();
        if(m_array != NULL){
            delete [] m_array;
        }
        m_mutex.unlock();
    }
    // 判断队列是否已满
    bool full(){
        m_mutex.lock();
        if(m_size >= m_max_size){
            m_mutex.unlock();
            return true;
        }
        m_mutex.unlock();
        return false;
    }
    // 判断队列是否为空
    bool empty(){
        m_mutex.lock();
        if(m_size == 0){
            m_mutex.unlock();
            return true;
        }
        m_mutex.unlock();
        return false;
    }
    // 获取队列前端元素
    bool front(T &value){
        m_mutex.lock();
        if(m_size == 0){
            m_mutex.unlock();
            return false;
        }
        value = m_array[m_frout];
        m_mutex.unlock();
        return true;
    }
    // 获取队列后端元素
    bool back(T &value){
        m_mutex.lock();
        if(m_size == 0){
            m_mutex.lock();
            return false;
        }
        value = m_array[m_back];
        m_mutex.unlock();
        return true;
    }
    // 获取队列当前元素数量
    int size(){
        int tmp = 0;
        m_mutex.lock();
        tmp = m_size;
        m_mutex.unlock();
        return tmp;
    }
    // 获取队列的最大容量
    int max_size(){
        int tmp = 0;
        m_mutex.lock();
        tmp = m_max_size;
        m_mutex.unlock();
        return tmp;
    }
    // 向队列中推送新元素
    bool push(const T &item) {
        m_mutex.lock();
        if (m_size >= m_max_size) {
            m_cond.broadcast();
            m_mutex.unlock();
            return false;
        }

        m_back = (m_back + 1) % m_max_size;
        m_array[m_back] = item;

        m_size++;
        m_cond.broadcast();
        m_mutex.unlock();
        return true;
    }
    // 从队列中弹出元素
    bool pop(T &item) {
        m_mutex.lock();
        while (m_size <= 0) {
            if (!m_cond.wait(m_mutex.get())) {
                m_mutex.unlock();
                return false;
            }
        }

        m_frout = (m_frout + 1) % m_max_size;
        item = m_array[m_frout];
        m_size--;
        m_mutex.unlock();
        return true;
    }
    // 在指定的超时时间内从队列中弹出元素
    bool pop(T &item, int ms_timeout) {
        struct timespec t = {0, 0};
        struct timeval now = {0, 0};
        gettimeofday(&now, NULL);
        m_mutex.lock();
        if (m_size <= 0) {
            t.tv_sec = now.tv_sec + ms_timeout / 1000;
            t.tv_nsec = (ms_timeout % 1000) * 1000;
            if (!m_cond.timewait(m_mutex.get(), t)) {
                m_mutex.unlock();
                return false;
            }
        }

        if (m_size <= 0)
        {
            m_mutex.unlock();
            return false;
        }

        m_frout = (m_frout + 1) % m_max_size;
        item = m_array[m_frout];
        m_size--;
        m_mutex.unlock();
        return true;
    }
private:
    locker m_mutex; // 互斥锁
    cond m_cond;    // 条件变量
    T *m_array;     // 存储队列元素的数组
    int m_size;     // 队列当前元素数量
    int m_max_size; // 队列的最大容量
    int m_frout;    // 队列前端的索引
    int m_back;     // 队列后端的索引
};

#endif