# 链表

## 一  链表简介

        链表是一种经典的数据结构，用于存储线性数据结构，如列表、队列和栈。它是由一系列节点组成的，每个节点都包含一个存储指针和一个数据变量。指针指向下一个节点，最后一个节点指向NULL。

        在C++中实现链表需要定义节点类。该类包含两个成员变量：一个指向下一个节点的指针和一个数据变量。然后定义链表类，该类包含一个指向链表头的指针和一些成员函数。

        一个指针域一个数据域，指针域指向下一节点的数据域；



## 二  代码

list.h

```c++
#pragma once
#include<vector>
#include<iostream>
using namespace std;
struct Node
{
    Node *next;
    int val;
    Node(int val)
    {
        next = nullptr;
        this->val = val;
    }

};

class List
{
    pHead = nullptr;
    pTail = nullptr;
public:
    List();
    List(std::vector<int> vec);

    void printList();
    Node* getHead();
    Node* getTail();
    void addNode(int val);
    bool delNode(int val);
    bool updataNode(int res ,int des);
    Node* findNode(int val);
};
```

list.cpp

```c++
#include "List.h"

List::List()
{
}

List::List(std::vector<int> vec)
{
    for (int i = 0; i < vec.size(); i++)
    {
        Node *node = new Node(vec[i]);// 通过vec 创建新节点,node存的是节点的地址
        if (pHead == nullptr) //如果创建的节点是链表的第一个节点那么让phead 和 ptail 都指向这个节点
        {
            pHead = node;
            pTail = node;
        }
        else
        {
            pTail->next = node;//把新节点加入到链表
            pTail = node; //移动ptail指针指向最后一个节点，也就是新节点node
        }
    }
}

void List::printList()
{
    Node *p = pHead;
    while (p)
    {
        std::cout << p->val << " ";
        p = p->next;
    }
}

Node* List::getHead()
{
    return pHead;
}
Node* List::getTail()
{
    return pTail;
}

void List::addNode(int val)
{
    Node *node = new Node(val);
    pTail->next = node;
    pTail = node;
}

bool List::delNode(int val)
{
    Node* fast = pHead;
    Node* slow = nullptr;

    while (fast!=nullptr)
    {

        if (fast->val == val)
        {
            if (fast == pHead)
            {
                pHead = pHead->next;
            }
            else
            {
                slow->next = fast->next;
            }
            return true;
        }
        slow = fast;
        fast = fast->next;
    }

    return false;
}

bool List::updataNode(int res, int des)
{
    Node * p =findNode(res);
    if (p)
    {
        p->val = des;
        return true;
    }

    return false;
}

Node* List::findNode(int val)
{
    Node *p = pHead;
    while (p)
    {
        if (p->val == val)
        {
            return p;
        }
        p = p->next;
    }
    return nullptr;
}
```

main.cpp

```c++
#include"List.h"

using namespace std;

int main()
{
    vector<int> vec = {1,2,3,4,5};
    List list(vec);
    list.printList();
    return 0;
}
```

### 2.1 头插法和尾插法

链表的头插法和尾插法主要指的是在不同的位置插入节点。头插法指将新节点插入链表的头部，而尾插法指将新节点插入链表的尾部。

下面是基于上一个例子修改后实现链表头插法和尾插法的示例代码：

```c++
#include <iostream>

using namespace std;

// 链表节点类
class Node {
public:
    int data;
    Node* next;

    Node(int data) : data(data), next(nullptr) {}
};

// 链表类
class LinkedList {
private:
    Node* head;

public:
    LinkedList() : head(nullptr) {}

    // 尾插法插入节点
    void appendNode(int data) {
        Node* newNode = new Node(data);
        if (head == nullptr) {
            head = newNode;
        } else {
            Node* cur = head;
            while (cur->next != nullptr) {
                cur = cur->next;
            }
            cur->next = newNode;
        }
    }

    // 头插法插入节点
    void prependNode(int data) {
        Node* newNode = new Node(data);
        if (head == nullptr) {
            head = newNode;
        } else {
            newNode->next = head;
            head = newNode;
        }
    }

    // 删除指定数据节点
    void deleteNode(int data) {
        if (head == nullptr) {
            return;
        }

        Node* cur = head;
        Node* prev = nullptr;

        while (cur != nullptr && cur->data != data) {
            prev = cur;
            cur = cur->next;
        }

        if (cur == nullptr) {
            return;
        }

        if (prev == nullptr) {
            head = cur->next;
        } else {
            prev->next = cur->next;
        }

        delete cur;
    }

    // 打印链表
    void printList() {
        Node* cur = head;
        while (cur != nullptr) {
            cout << cur->data << " ";
            cur = cur->next;
        }
        cout << endl;
    }
};

int main() {
    LinkedList linkedList;

    // 尾插法插入节点
    linkedList.appendNode(1);
    linkedList.appendNode(3);
    linkedList.appendNode(5);
    linkedList.appendNode(7);
    linkedList.printList();  // 输出: 1 3 5 7

    // 头插法插入节点
    linkedList.prependNode(2);
    linkedList.prependNode(4);
    linkedList.printList();  // 输出: 4 2 1 3 5 7

    // 删除指定节点
    linkedList.deleteNode(3);
    linkedList.printList();  // 输出: 4 2 1 5 7
    linkedList.deleteNode(4);
    linkedList.printList();  // 输出: 2 1 5 7

    return 0;
}
```

        在这个示例代码中，我们定义了appendNode方法和prependNode方法来分别实现链表的尾插法和头插法。此外，我们还定义了deleteNode方法来删除链表中给定数据的节点。在主函数中，我们首先使用尾插法插入一些节点，然后使用头插法插入一些节点，并最终删除一些节点并输出链表。

## 三  Leetcode

### 3.1  合并两有序链表

```cpp
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
        //创建虚拟头节点pHead和pTail
        ListNode* pHead = new ListNode(0);
        ListNode* ptail = pHead;
        //循环  判断链表是否为空
        while (list1 != nullptr && list2 != nullptr) {
            //比较list1和list2两个节点的大小
            if (list1->val < list2->val) {
                //连接较小的链表
                ptail->next = list1;
                //list1向前移动
                list1 = list1->next;
            }
                  else {
                //连接较小的链表
                ptail->next = list2;
                //list2向前移动
                list2 = list2->next;
            }
            //pTail向前移动
            ptail = ptail->next;
        }
        //判断list1和list2两个链表是否为空，不为空的那组pTail持续向前移动
        ptail->next = list1 == nullptr ? list2 : list1;
        return pHead->next;
    }
};
```

### 3.2  删除排序链表中的重复元素

```c++
class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        if (!head) return head;
        ListNode* pHead = head;
        while (pHead->next) {
            if (pHead->val == pHead->next->val) {
                pHead->next = pHead->next->next;
            }
            else {
                pHead = pHead->next;
            }
        }
        return head;
    }
};
```

### 3.3  移除链表元素

```c++
class Solution {
public:
    ListNode* removeElements(ListNode* head, int val) {
        //判断是否为空链表或者链表一个数且同为val
        while (head && head->val == val) {
            head = head->next;
        }
        //定义快慢指针（双指针）
        ListNode* fast = head;
        ListNode* slow = head;
        //执行
        while (fast) {
            if (fast->val == val) {
                //移除链表元素
                slow->next = fast->next;
            }
            else {
                slow = fast;
            }
            //快指针指向下一个地址
            fast = fast->next;
        }
        return head;
    }
};
```

### 3.4  反转链表

```c++
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        ListNode* cur = NULL, *pre = head;
        while (pre != NULL) {
            ListNode* temp = pre->next;
            pre->next = cur;
            cur = pre;
            pre = temp;
        }
        return cur;
    }
};
```

### 3.5  回文链表

```c++
class Solution {
public:
    bool isPalindrome(ListNode* head) {
        if (head == nullptr || head->next == nullptr) {
            return true;
        }
        //快慢指针找到中间节点
        ListNode* slow = head;
        ListNode* fast = head;
        while (fast->next != nullptr && fast->next->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
        }
        //反转后半部分链表
        ListNode* pre = nullptr;
        ListNode* cur = slow->next;
        while (cur != nullptr) {
            ListNode* next = cur->next;
            cur->next = pre;
            pre = cur;
            cur = next;
        }
        //比较前半部分和反转后的后半部分链表
        ListNode* p1 = head;
        ListNode* p2 = pre;
        while (p2 != nullptr) {
            if (p1->val != p2->val) {
                return false;
            }
            p1 = p1->next;
            p2 = p2->next;
        }
        return true;
    }
};
```
