/***********************************************************************************
# File Name:    fileoper.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年04月28日 星期日 10时02分17秒
 **********************************************************************************/

#include <stdio.h>
#include<string.h> 
#include<stdbool.h>
#include<stdlib.h>
int main() {

  FILE *pf = fopen("1.txt", "r");
  FILE *pf1 = fopen("2.txt", "w");
  if (pf == NULL || pf1 == NULL) {

    perror("open()");
    exit(1);
  }
  char ch;
  char ch1;
  while ((ch = fgetc(pf)) != EOF) {
	fputc(ch,pf1);
    if (ch == 'a') {
      fputs("hello word", pf1);
    }
  }
remove("1.txt");
rename("2.txt","1.txt");

  fclose(pf);
  fclose(pf1);
  pf, pf1 = NULL;

  return 0;
}
