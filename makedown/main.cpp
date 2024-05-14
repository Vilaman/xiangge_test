#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include "block_queue.h"
#include "log.h"
#include "sql_connection_pool.h"

// 生产者线程函数
void* producer(void* arg) {
    block_queue<int>* queue = static_cast<block_queue<int>*>(arg);
    for (int item = 0; item < 20; item++) {
        queue->push(item);
        LOG_INFO("生产者产生数据i = %d", item);
        usleep((rand() % 5 + 1) * 50000);
    }
    return nullptr;
}

// 消费者线程函数
void* consumer(void* arg) {
    block_queue<int>* queue = static_cast<block_queue<int>*>(arg);
    for (int item = 0; item < 20; item++) {
        queue->pop(item);
        LOG_INFO("消费者处理数据i = %d", item);
    }
    return nullptr;
}

int main() {
    block_queue<int> queue;
    pthread_t tid_prod, tid_cons;
    pthread_create(&tid_prod, nullptr, producer, &queue);
    pthread_create(&tid_cons, nullptr, consumer, &queue);
    pthread_join(tid_prod, nullptr);
    pthread_join(tid_cons, nullptr);
    return 0;
}
