#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <pthread.h>

struct student
{
    int nu;
    struct student* next;
};

struct student* head = NULL;
pthread_mutex_t mutex;
pthread_cond_t cond;

//生产者
void* producer(void* arg)
{
    int i = 0;
    while(1)
    {
        struct student* node = (struct student*)malloc(sizeof(struct student));
        node->nu = i++;
        printf("+++++++++++++++++生产者产生数据i = %d\n", node->nu);
        pthread_mutex_lock(&mutex);
        //连彪插入，头插
        node->next = head;
        head = node;
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond);
        usleep((rand()%5 + 1) * 50000);
    }

    return NULL;
}
//消费者
void* consumer(void* arg)
{
    struct student* node;
    while(1)
    {
        pthread_mutex_lock(&mutex);
        while(head == NULL)
        {
            //阻塞在条件变量上
            pthread_cond_wait(&cond, &mutex);
        }
        //摘下头节点
        node = head;
        head = head->next;
        pthread_mutex_unlock(&mutex);
        printf("-----消费者处理数据i = %d\n", node->nu);
        free(node);
        usleep((rand()%5 + 1) * 50000);
    }
    return NULL;
}


int main(int argc, char* argv[])
{
    srand(time(NULL));
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    pthread_t tid1;
    pthread_t tid2;
    pthread_create(&tid1, NULL, producer, NULL);
    pthread_create(&tid2, NULL, consumer, NULL);
    pthread_create(&tid2, NULL, consumer, NULL);
    pthread_create(&tid2, NULL, consumer, NULL);
    pthread_create(&tid2, NULL, consumer, NULL);
    pthread_create(&tid2, NULL, consumer, NULL);

    while(1);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    pthread_exit(NULL);
}

