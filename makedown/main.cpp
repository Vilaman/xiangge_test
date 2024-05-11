#include <iostream>
#include <vector>
#include <pthread.h>
#include "locker.h" // 包含我们之前定义的locker类（互斥锁、信号量、条件变量）

const int MAX_ITEMS = 10; // 定义缓冲区的最大容量
int buffer[MAX_ITEMS]; // 定义一个整型数组作为缓冲区
int count = 0; // 记录缓冲区中产品的数量

locker mutex; // 创建一个互斥锁实例，保护缓冲区的访问
sem empty(MAX_ITEMS); // 初始化empty信号量，初始值为MAX_ITEMS，表示缓冲区为空的槽数量
sem full(0); // 初始化full信号量，初始值为0，表示缓冲区中产品的数量

// 生产者函数
void* producer(void* arg) {
    for(int i = 0; i < 20; ++i) {
        empty.wait(); // 在空缓冲区位置减少之前等待。如果没有空位，则阻塞。
        mutex.lock(); // 进入临界区之前加锁
        // 生产操作：添加一个产品到缓冲区
        buffer[count++] = i;
        std::cout << "Produced: " << i << std::endl;
        mutex.unlock(); // 离开临界区后解锁
        full.post(); // 完成生产，增加full信号量，表示产品数量增加
    }
    return NULL;
}

// 消费者函数
void* consumer(void* arg) {
    for(int i = 0; i < 20; ++i) {
        full.wait(); // 在有产品可消费之前等待。如果没有产品，则阻塞。
        mutex.lock(); // 进入临界区之前加锁
        // 消费操作：从缓冲区取出一个产品
        int item = buffer[--count];
        std::cout << "Consumed: " << item << std::endl;
        mutex.unlock(); // 离开临界区后解锁
        empty.post(); // 完成消费，增加empty信号量，表示有更多空位
    }
    return NULL;
}

int main() {
    pthread_t prod, cons;
    // 创建生产者和消费者线程
    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    // 等待线程结束
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    return 0;
}