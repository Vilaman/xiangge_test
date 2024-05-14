/***********************************************************************************
# File Name:    node.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年04月24日 星期三 14时48分02秒
 **********************************************************************************/

#include <stdio.h>
#include<string.h> 
#include<stdbool.h>
#include<stdlib.h>
typedef int typenode;

typedef struct Node {
  typenode data;
  struct Node *next;

} Node;

Node *initnode(typenode data);
void appendNode(Node **n,typenode data);
void showNode(Node *n);
void Insertnode();
int sumNode(Node *phead);
void DelNode(Node *phead,int index);
void Changenode();
void shownodeData();
bool isDataexist();
void Nodefree();


int main() {
  Node *plist = NULL;
  appendNode(&plist, 6);
  appendNode(&plist, 7);
  appendNode(&plist, 8);
  appendNode(&plist, 9);
  appendNode(&plist, 10);
  showNode(plist);
  printf("--------------------\n");
  Insertnode(&plist, 11, 5);
  showNode(plist);
  printf("--------------------\n");
  DelNode(plist, 4);
  Changenode(&plist, 22, 0);
  Changenode(&plist, 25, 1);
  Changenode(&plist, 24, 6);
  DelNode(plist, 11);
  showNode(plist);
  shownodeData(plist,0);
  shownodeData(plist,2);
  shownodeData(plist,1);
  int isexist=isDataexist(plist,8);
  int isexist1=isDataexist(plist,22);
  int isexist2=isDataexist(plist,6);
  printf("%d,%d,%d\n",isexist,isexist1,isexist2);
  Nodefree(&plist);
  shownodeData(plist,1);

  return 0;
}


//初始化newnode
Node *initnode(typenode data) {
  Node *newnode = (Node *)malloc(sizeof(Node));
  newnode->data = data;
  newnode->next = NULL;
  return newnode;
}



//尾插数据
void appendNode(Node **phead, typenode data) {
  Node *newnode = initnode(data);//调用初始化newnode
  if (*phead == NULL) {
    *phead = newnode;
  } else {
    //申请新数据的地址空间
    Node *tail = *phead; //定义尾和头相等 方便向下寻找tail的真正地址
    while (tail->next !=
           NULL) { //当尾的next不为NULL就一直向下找 直到找到尾的NULL为止
      tail = tail->next;
    }
    tail->next = newnode;
    //找到tail过后此时tail->next指向的是NULL 把newnode的节点给他
  }
}

//遍历当前链表的data
void showNode(Node *phead) {
  Node *cur = phead;//定义一个cur节点 指向phead
  while (cur != NULL) {//从当前链表的第一个节点开始向下遍历

    printf("%d\n", cur->data);
    cur = cur->next;
  }
}


//下标插入数据
void Insertnode(Node **phead, typenode data, int index) {

  int count = sumNode(*phead);
  if (index > count) {
    printf("输入下标过大\n");
    return;
  }
  Node *newnode = initnode(data); //初始化newnode
  if (*phead == NULL) {
    *phead = newnode; //判断如果插的位
  } else {

    Node *cur = *phead;
    for (int i = 0; i < index - 1; i++) {
      cur = cur->next;
    }
    newnode->data = data;
    newnode->next = cur->next;
    cur->next = newnode;
  }
}

//统计链表中的个数
int sumNode(Node *phead) {
  int count = 0;
  Node *cur = phead;
  if (cur == NULL) {
    return count;//如果当前链表为空则返回0
  }
  do {
    count++;
    cur = cur->next;
  } while (cur!=NULL);
  return count;
}

//删除下标对应的链表
void DelNode(Node *phead, int index) {
  int count = sumNode(phead);
  if (index > count - 1 || index < 0) {
    printf("输入下标不合法\n");
    return;
  }
  if (phead == NULL) {
    printf("当前链表不存在数据\n");
    return;
  }
  Node *cur = phead;
  for (int i = 0; i < index - 1; i++) {
    cur = cur->next;
  }
  Node *Nextnode = cur->next; //定义index当前的节点位置
  cur->next =cur->next->next; //当前index指向的下个节点就赋值给他的上个节点也就是我们拿到的cur
  free(Nextnode); //手动释放index节点
  Nextnode = NULL;
}

//修改下标对应数据
void Changenode(Node **phead, typenode data, int index) {
  int count = sumNode(*phead);
  if (index > count - 1 || index < 0) {
    printf("输入下标不合法\n");
    return;
  }
  if (phead == NULL) {
    printf("当前链表不存在数据\n");
    return;
  }
  Node *cur = *phead;
  if (index == 0) {
    cur->data = data;
  } else {

    for (int i = 0; i < index - 1; i++) {
      cur = cur->next;
    }
    cur->next->data = data;
	printf("当前下标%d数据修改为%d\n",index,cur->next->data);
  }
}

//查看下标对应的数据
void shownodeData(Node *phead, int index) {

  if (phead == NULL) {
    printf("当前链表不存在数据\n");
    return;
  }
  int count = sumNode(phead);
  if (index > count - 1 || index < 0) {
    printf("当前下标不存在\n");
    return;
  }
  Node *cur = phead;
  for (int i = 0; i < index - 1; i++) {
    cur = cur->next;
  }
  printf("当前%d下标的值为%d\n",index, cur->next->data);
  return;
}

//判断数据是否存在链表中
bool isDataexist(Node *phead, typenode data) {
  if (phead == NULL) {
    printf("当前链表不存在数据\n");
    return false;
  }
  Node *cur = phead;
  do {
    if (cur->data == data) {
      printf("当前数据%d存在链表中\n",data);
      return true;
    }
    cur = cur->next;
  } while (cur != NULL);

  printf("当前数据%d不存在链表中\n",data);
  return false;
}

//清空链表
void Nodefree(Node **phead) {
  Node *cur = *phead;
  Node *Next = NULL;
  while (cur != NULL) {
    Next = cur->next;//先用next拿到下一个节点 不然直接释放会找不到下一个节点的值
    free(cur);//释放掉当前的值
    cur = Next;//将上面拿到的next节点给到cur
  }
  *phead = NULL;
}
