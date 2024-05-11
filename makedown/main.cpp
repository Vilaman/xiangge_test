#include <iostream>
#include <vector>
#include <pthread.h>
#include <unistd.h>
#include "locker.h" // 包含我们之前定义的locker类（互斥锁、信号量、条件变量）

struct student {
    int nu;
    struct student* next;
};

struct student* head = NULL;
locker mutex;
cond m_cond;
sem full(0);

//生产者
void* producer(void* arg) {
    int i = 0;
    while(1) {
        struct student* node = (struct student*)malloc(sizeof(struct student));
        node->nu = i++;
        printf("+++++++++++++++++生产者产生数据i = %d\n", node->nu);
        mutex.lock();
        //连彪插入，头插
        node->next = head;
        head = node;
        mutex.unlock();
        m_cond.signal();
        usleep((rand()%5 + 1) * 50000);
    }

    return NULL;
}

//消费者
void* consumer(void* arg) {
    struct student* node;
    while(1) {
        mutex.lock();
        while(head == NULL) {
            //阻塞在条件变量上
            m_cond.wait(mutex.get());
        }
        //摘下头节点
        node = head;
        head = head->next;
        mutex.unlock();
        printf("-----消费者处理数据i = %d\n", node->nu);
        free(node);
        usleep((rand()%5 + 1) * 50000);
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