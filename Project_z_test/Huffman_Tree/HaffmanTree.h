#pragma once
/*
哈夫曼树
*/

#include<queue>
#include<vector>
using namespace std;

template <class T>
struct HuffmanTreeNode //构建哈夫曼树的节点
{
	HuffmanTreeNode<T>* left; //左指针域
	HuffmanTreeNode<T>* right; //右指针域
	HuffmanTreeNode<T>* parent;//双亲节点指针域
	T weight; //权值

	HuffmanTreeNode(const T& w) //构造函数
		: left(nullptr)
		, right(nullptr)
		, parent(nullptr)
		, weight(w)
	{}
};

//因为使用默认的优先级队列采用的是less的比较方式，建的是大堆
//因此用到仿函数的方式给priority_queue的第三个参数传参,需要自定义比较方式，按照节点中的权值大小来比较
template <class T>
struct Compare
{
	typedef HuffmanTreeNode<T> Node;
	bool operator()(const Node* left, const Node* right)
	{
		return left->weight > right->weight;
	}
};

template <class T>
class HuffmanTree
{
	typedef HuffmanTreeNode<T> Node;//重命名
public:
	HuffmanTree() //构造函数
		:root(nullptr)
	{}

	//析构函数
	~HuffmanTree()
	{
		DestroyHuffmanTree(root);
	}

	//哈夫曼树的创建
	void CreateHuffmanTree(const T array[], size_t size, const T& invalid)
	{
		//使用优先级队列来保存哈夫曼树
		std::priority_queue<Node*, vector<Node*>, Compare<T>> q;

		//1.使用权值创建二叉树森林
		for (size_t i = 0; i < size; ++i)
		{
			//使用if判断节点是否为0(无效节点)
			if (array[i] != invalid)
				q.push(new Node(array[i]));
		}
		//循环进行以下步骤
		while (q.size() > 1)
		{
			//1.取权值最小的两颗二叉树
			Node* left = q.top();
			q.pop();
			Node* right = q.top();
			q.pop();
			//2.构建新二叉树，以左右子树中的权值之和作为新二叉树的权值
			Node* parent = new Node(left->weight + right->weight);
			parent->left = left;
			parent->right = right;
			left->parent = parent;
			right->parent = parent;
			//3.将新二叉树放入到二叉树森林中
			q.push(parent);
		}
		root = q.top();
	}

	Node* GetRoot()
	{
		return root;
	}
	//销毁哈夫曼树
	void DestroyHuffmanTree(Node*& proot)
	{
		if (proot != nullptr)
		{
			DestroyHuffmanTree(proot->left);
			DestroyHuffmanTree(proot->right);
			proot = nullptr;
		}
	}
private:
	Node* root;//根节点
};
