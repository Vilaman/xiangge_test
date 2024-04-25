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

