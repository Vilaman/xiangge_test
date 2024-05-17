#pragma once
#include<iostream>
#include<string>
#include<algorithm>
#include"HaffmanTree.h"
using namespace std;

struct ByteInfo {
	unsigned char ch;//字符
	int count;//字符出现的次数
	string strcode;//字符对应的哈夫曼编码
	//构造函数
	ByteInfo(int BIcount = 0)
		:count(BIcount)
	{}
	//在使用ByteInfo实例化对象时，需要对+, >, !=, ==进行重载
	ByteInfo operator+(const ByteInfo& b)const {
		ByteInfo tmp;
		tmp.count = count + b.count;
		return tmp;
	}
	bool operator>(const ByteInfo& b)const {
		if (count > b.count) {
			return true;
		}
		else {
			return false;
		}
	}
	bool operator!=(const ByteInfo& b)const {
		if (count != b.count) {
			return true;
		}
		else {
			return false;
		}
	}
	bool operator==(const ByteInfo& b)const {
		if (count == b.count) {
			return true;
		}
		else {
			return false;
		}
	}
};

class FileCompress {
public:
	//构造函数
	FileCompress();
	//压缩方法
	bool Compress(const string& filepath);
	//解压缩方法
	bool UnCompress(const string& filepath);
	void menu();
private:
	void GetLine(FILE* FIn, string& strContent);//C语言中没有获取一行的函数，需自己进行封装
	void CreateHuffmanCode(HuffmanTreeNode<ByteInfo>* root);
	void CompressInfo(FILE* fl, const string& filepath);
	//文件在磁盘中是以字节方式存储的，因此使用
	//包含256个ByteInfo类型的数组来保存字节出现的信息
	ByteInfo filebyteinfo[256];
};
