/***********************************************************************************
# File Name:    strnode.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年04月25日 星期四 11时02分52秒
 **********************************************************************************/

#include <stdio.h>
#include<string.h> 
#include<stdbool.h>
#include<stdlib.h>
#include"mySLNode.h"


int main() {
  Node *str = NULL;
  appendNode(&str, "abcabc");
  appendNode(&str, "agcabc");
  appendNode(&str, "afcabc");
  appendNode(&str, "aecabc");
  appendNode(&str, "adbc");
  showNode(str);
  printf("--------------------\n");
//  Changenode(&str, "12344", 0);
  // Insertnode(&str, "cccc", 2);
  DelNode(&str, 0);
  DelNode(&str, 0);
  showNode(str);
  int count = sumNode(str);
  printf("%d\n", count);
  return 0;
}
