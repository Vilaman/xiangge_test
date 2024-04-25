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
