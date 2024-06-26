/***********************************************************************************
# File Name:    mySLNode.h
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年04月25日 星期四 10时24分51秒
 **********************************************************************************/
//防止多次引入头文件重复定义

#ifndef MYSLNODE_H
#define MYSLNODE_H
#define SIZE 100

typedef struct User {
  char username[20];
  char password[20];
  char sex[5];
  int age;
} User;

typedef User typenode;


typedef struct Node {
  typenode *data;
  struct Node *next;

} Node;


Node *initnode(typenode *data);
void appendNode(Node **phead,typenode *data);
void showNode(Node *phead);
void Insertnode(Node **phead,typenode *data,int index);
int sumNode(Node *phead);
void Changenode(Node **phead, typenode* data, int index);
void DelNode(Node **phead,int index);
User *shownodeData(Node *phead, int index);
int isDataexist(Node *phead, char *name,char *pass);
void Nodefree(Node **phead);





#endif
