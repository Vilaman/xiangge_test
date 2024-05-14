/***********************************************************************************
# File Name:    vrify.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年04月25日 星期四 23时36分43秒
 **********************************************************************************/

#include <stdio.h>
#include<string.h> 
#include<stdbool.h>
#include<stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
int main(){
struct a{
char *c;
};
struct a b={"rtyui"};

char *parr;
scanf(" %s",parr);
printf("ghjk");

if(strcmp(parr,b.c)!=0){
printf("1ghjj");
}

    return 0;
}
