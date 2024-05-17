#define _CRT_SECURE_NO_WARNINGS
#include"File_Compress_UnCompress.h"
#include"HaffmanTree.h"

FileCompress::FileCompress() {
	for (int i = 0; i < 256; i++) {
		filebyteinfo[i].ch = i;
	}
}

bool FileCompress::Compress(const string& filepath) {
	//1.获取原文件中每个字节出现的次数
	//打开文件
	//不存在从string到constchar*的转换函数的解决方法
	//使用cstr()函数，cstr函数的返回值是const
	//c_str(()函数返回一个指向正规C字符串的指针..
	FILE* fp = fopen(filepath.c_str(), "rb");
	if (fp == nullptr) {//如果文件为空或者没有文件,输出"fopen: No such file or directory"
		perror("fopen");
		return false;
	}
	//读取文件内容，采用循环，一次读取1024个字节
	unsigned char readbuff[1024];//设置缓冲区
	while (1) {
		//fread函数的返回值，表示成功读取的字节数量
		//用read_size来接收成功读取的字节数
		size_t read_size = fread(readbuff, 1, 1024, fp);
		if (read_size == 0) {
			break;//read_size = 0表示已经读到了文件末尾
		}
		//统计出现字符的次数
		for (size_t i = 0; i < read_size; ++i) {
			//利用字符的ASCII码值作为数组的下标统计次数
			filebyteinfo[readbuff[i]].count++;
		}
	}
	//2.根据字节出现的频次信息构建Huffman树
	HuffmanTree<ByteInfo> ht;
	ByteInfo invalid;
	ht.CreateHuffmanTree(filebyteinfo, 256, invalid);
	//3.获取Huffman编码
	CreateHuffmanCode(ht.GetRoot());
	string filename_Compress;
	cout << "输入需要解压缩文件后的文件名" << endl;
	cin >> filename_Compress;
	//string PostFixs = ".bmp";
	//filename_Compress += PostFixs;
	//4.保存压缩信息(方便解压缩时对信息进行读取)
	FILE* fl = fopen(filename_Compress.c_str(), "wb");//fl文件指针指向打开的文件2.txt
	CompressInfo(fl, filepath);
	//5.使用Huffman编码来改写文件
	//在对文件进行读操作后，文件指针的位置指向文件末尾
	//注意，这里需要将文件指针重置到文件头部
	fseek(fp, 0, SEEK_SET);//使用rewind(fp)函数也行
	unsigned char ch = 0;//记录按位或后的字符
	int bitcount = 0;//记录按位或的比特位长度
	//采用循环读文件内容
	while (1) {
		//fread函数的返回值，表示成功读取的字节数量
		//用read_size来接收成功读取的字节数
		size_t read_size = fread(readbuff, 1, 1024, fp);
		if (read_size == 0) {
			break;//read_size = 0表示已经读到了文件末尾
		}

		for (size_t i = 0; i < read_size; ++i) {
			//获取filebyteinfo数组中的编码
			string& strcode = filebyteinfo[readbuff[i]].strcode;
			for (size_t j = 0; j < strcode.size(); j++) {
				ch <<= 1;//先将ch左移一位
				if (strcode[j] == '1') {//再让strcode的每个比特位与1进行或操作
					ch |= 1;
				}
				bitcount++;//每按位或一次就对bitcount++
				if (bitcount == 8) {//一个字节放满8个比特位
					fputc(ch, fl);//往文件中进行写操作
					bitcount = 0;//比特位计数置0
				}
			}
		}
	}
	//检测ch放置的比特位个数，不够8个继续往文件中写
	if (bitcount > 0 && bitcount < 8) {
		ch <<= (8 - bitcount);
		fputc(ch, fl);
	}
	int fpSize = ftell(fl);
	int outSize = ftell(fp);
	cout << "\n***********************************\n" << endl;
	cout << "压缩前：" << fpSize * 1024 << "字节" << endl;
	cout << "压缩后：" << outSize * 1024 << "字节" << endl;
	cout << "***********************************" << endl;
	fclose(fp);//关闭文件
	fclose(fl);
	cout << "压缩成功" << endl;
	return true;
}

bool FileCompress::UnCompress(const string& filepath) {
	//1.获取压缩信息
	FILE* FIn = fopen(filepath.c_str(), "rb");//以读的方式打开文件
	if (FIn == nullptr) {//如果文件为空或者没有文件,输出"fopen: No such file or directory"
		perror("fopen");
		return false;
	}
	//①读后缀
	string PostFix;
	GetLine(FIn, PostFix);
	//②读总行数，读取
	string strContent;
	GetLine(FIn, strContent);
	size_t linecount = atoi(strContent.c_str());
	strContent = "";//先将strContent清空
	for (size_t i = 0; i < linecount; ++i) {
		GetLine(FIn, strContent);
		if (strContent == "") {//当读取到的是一个换行(对多行数据进行处理)
			strContent += "\n";
			GetLine(FIn, strContent);
		}
		//以strContent[0]即出现的字符作为filebyteinfo数组的下标
		filebyteinfo[(unsigned char)strContent[0]].count = atoi(strContent.c_str() + 2);//+2的目的是向后偏移两个位置
		strContent = "";
	}
	//2.还原哈夫曼树
	HuffmanTree<ByteInfo> ht;
	ByteInfo invalid;
	ht.CreateHuffmanTree(filebyteinfo, 256, invalid);
	//3.读取压缩数据，进行解压缩
	string filename_UnCompress;
	cout << "输入需要解压缩文件后的文件名" << endl;
	cin >> filename_UnCompress;
	//filename_UnCompress += PostFix;
	FILE* fout = fopen(filename_UnCompress.c_str(), "wb");
	unsigned char readbuff[1024];//缓冲区
	unsigned char bitcount = 0;
	HuffmanTreeNode<ByteInfo>* cur = ht.GetRoot();//cur指向哈夫曼树的根节点
	const int FileSize = cur->weight.count;//根节点的权值即位原文件的大小
	int CompressSize = 0;
	while (1) {
		size_t read_size = fread(readbuff, 1, 1024, FIn);
		if (read_size == 0) {
			break;
		}
		for (size_t i = 0; i < read_size; i++) {
			unsigned char ch = readbuff[i];//ch来保存压缩后的每一个字节
			bitcount = 0;
			while (bitcount < 8) {
				if (ch & 0x80) {//每个比特位与1000 0000按位与
					cur = cur->right;
				}
				else {
					cur = cur->left;
				}
				if (cur->left == nullptr && cur->right == nullptr) {//走到了叶子节点的位置
					fputc(cur->weight.ch, fout);
					cur = ht.GetRoot();//回到根节点的位置
					CompressSize++;
					if (CompressSize == FileSize) {//判断解压缩后的结果与原文件大小是否相同
						break;
					}
				}
				bitcount++;
				ch <<= 1;
			}
		}
	}
	int fpSize = ftell(fout);
	int outSize = ftell(FIn);
	cout << "\n***********************************\n" << endl;
	cout << "解压缩前：" << fpSize * 1024 << "字节" << endl;
	cout << "解压缩后：" << outSize * 1024 << "字节" << endl;
	cout << "***********************************" << endl;
	fclose(FIn);
	fclose(fout);
	cout << "解压缩成功" << endl;
	return true;
}

void FileCompress::GetLine(FILE* FIn, string& strContent) {
	unsigned char ch;
	while (!feof(FIn)) {
		ch = fgetc(FIn);
		if (ch == '\n') {
			break;
		}
		else {
			strContent += ch;
		}
	}
}

void FileCompress::CreateHuffmanCode(HuffmanTreeNode<ByteInfo>* root) {
	if (root == nullptr) {
		return;
	}
	//遍历到叶子节点所经过的路径就是Huffman编码
	if (root->left == nullptr && root->right == nullptr) {
		//使用从叶子节点找到根节点的方式获取哈夫曼编码
		HuffmanTreeNode<ByteInfo>* cur = root;//root标记叶子节点
		HuffmanTreeNode<ByteInfo>* parent = cur->parent;//parent为叶子节点的双亲
		//这行代码可能不容易理解，分析：
		//我们遍历Huffman树获取到的编码最终要保存到filebyteinfo数组，
		//数组中每个元素为ByteIofo结构体，需要保存到结构体中的strcode中
		//以当前节cur点中的权值所对应的字符下标作为filebyteinfo数组的下标
		//这里使用string&是引用，相当于取了别名
		string& strcode = filebyteinfo[cur->weight.ch].strcode;
		//走到根节点时双亲为空
		while (parent != nullptr) {
			if (cur == parent->left) {
				strcode += '0';
			}
			else {
				strcode += '1';
			}
			//cur和双亲parent继续向上标记
			cur = parent;
			parent = cur->parent;
		}
		//从叶子节点向上走到根节点获取的Huffman编码为反的，需要进行逆置
		reverse(strcode.begin(), strcode.end());
	}
	//递归遍历整个huffman树
	CreateHuffmanCode(root->left);
	CreateHuffmanCode(root->right);
}

void FileCompress::CompressInfo(FILE* fl, const string& filepath) {
	//1.截取文件后缀，如：1.txt需要截取txt
	string PostFix = filepath.substr(filepath.rfind('.'));//substr截取字串，rfind用来从后往前寻找.到末尾
	PostFix += "\n";//截取完加上换行让一条信息占一行
	fwrite(PostFix.c_str(), 1, PostFix.size(), fl);//往文件中写
	//2.记录频次信息占的总行数及频次信息
	size_t LineCount = 0;//记录行数
	string chcount;//字符出现次数
	for (size_t i = 0; i < 256; ++i) {
		if (filebyteinfo[i].count > 0) {
			chcount += filebyteinfo[i].ch;
			chcount += ':';
			chcount += to_string(filebyteinfo[i].count);//将数字转换成字符串
			chcount += "\n";
			LineCount++;
		}
	}
	string TLcount = to_string(LineCount);
	TLcount += "\n";
	fwrite(TLcount.c_str(), 1, TLcount.size(), fl);
	fwrite(chcount.c_str(), 1, chcount.size(), fl);
}

