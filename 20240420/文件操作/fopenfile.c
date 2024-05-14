/***********************************************************************************
# File Name:    fopenfile.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年04月26日 星期五 12时20分46秒
 **********************************************************************************/

#include <stdio.h>
#include<string.h> 
#include<stdbool.h>
#include<stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
int main() {

  FILE *fp1 = fopen("message.txt", "r");
  FILE *fp2 = fopen("READ.txt", "w+");
  if (fp1 == NULL) {
    perror("fopen()");
    exit(0);
  }
/*1  char ch;
  while ((ch = fgetc(fp1)) != EOF) {
  fputc(ch,fp2);*/

/*2  
  char buf[3];
  char *f1;
  while((f1=fgets(buf,sizeof(buf),fp1))!=NULL){
	fputs(buf,fp2);
  }*/
char buf[1024];
size_t count=fread(buf,1,sizeof(buf),fp1);
if(count<sizeof(buf)){
buf[count]='\0';
}
printf("%ld",count);
fwrite(buf,1,count,fp2);



  return 0;
}
