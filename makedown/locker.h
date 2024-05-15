#ifndef LOCKER_H
#define LOCKER_H

#include <exception>
#include <semaphore.h>
#include <pthread.h>

class sem {
public:
    sem(int value = 0) {
        if (sem_init(&m_sem, 0, value) != 0) {
            throw std::exception();
        }
    }
    ~sem() {
        sem_destroy(&m_sem);
    }
    bool wait() {
        return sem_wait(&m_sem) == 0;
    }
    bool post() {
        return sem_post(&m_sem) == 0;
    }
private:
    sem_t m_sem;
};

class locker {
public:
    locker() {
        if (pthread_mutex_init(&m_mutex, NULL) != 0) {
            throw std::exception();
        }
    }
    ~locker() {
        pthread_mutex_destroy(&m_mutex);
    }
    bool lock() {
        return pthread_mutex_lock(&m_mutex);
    }
    bool unlock() {
        return pthread_mutex_unlock(&m_mutex);
    }
    pthread_mutex_t* get() {
        return &m_mutex;
    }
private:
    pthread_mutex_t m_mutex;
};

class cond {
public:
    cond() {
        if (pthread_cond_init(&m_cond, NULL) != 0) {
            throw std::exception();
        }
    }
    ~cond() {
        pthread_cond_destroy(&m_cond);
    }
    bool wait(pthread_mutex_t* m_mutex) {
        int ret = pthread_cond_wait(&m_cond, m_mutex);
        return ret == 0;
    }
    bool timewait(pthread_mutex_t* m_mutex, struct timespec t) {
        return pthread_cond_timedwait(&m_cond, m_mutex, &t);
    }
    bool signal() {
        return pthread_cond_signal(&m_cond) == 0;
    }
    bool broadcast() {
        return pthread_cond_broadcast(&m_cond) == 0;
    }
private:
    pthread_cond_t m_cond;
};

#endif

/*
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
*/