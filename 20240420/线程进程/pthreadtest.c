/***********************************************************************************
# File Name:    pthreadtest.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月07日 星期二 14时53分28秒
 **********************************************************************************/

#include <stdio.h>
#include<string.h> 
#include<stdbool.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<pthread.h>

void *PTHREAD_A(void *);
void *PTHREAD_B(void *);
bool isexit=false;



int main(){

	pthread_t pid1;
	pthread_t pid2;
	

	int back1=pthread_create(&pid1,NULL,PTHREAD_A,NULL);
	if(back1!=0){

		perror("pthread_create()");
		exit(1);
	}else{
		
		puts("线程创建成功");

	}
	int back2=pthread_create(&pid2,NULL,PTHREAD_B,&pid1);
	if(back2!=0){

		perror("pthread_create()");
		exit(1);
	}else{
		
		puts("线程创建成功");

	}
	pthread_join(pid1,NULL);
	pthread_join(pid2,NULL);


    return 0;
}

void *PTHREAD_A(void *args) {
  pthread_t pid1 = pthread_self();
  printf("进程号为 %ld\n", pid1);
  for (int i = 1; i <= 100; i++) {

    printf("%d\n", i);
    sleep(2);
  }
}

void *PTHREAD_B(void *args) {
	pthread_t *tid=(pthread_t *)args;
  char input[20];
  pthread_t pid = pthread_self();
  printf("进程号为 %ld\n", pid);
  do {

    printf("请输入\n");
    scanf(" %s", input);
  } while (strcmp(input, "exit") != 0);
  pthread_cancel(*tid);
  pthread_exit(NULL);

}
