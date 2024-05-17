#include"File_Compress_UnCompress.h"
#include<vector>
void FileCompress::menu() {
	while (1) {
		printf("===========欢迎使用本程序===========\n");
		fflush(stdin);/*清空文件缓冲区*/
		cout << "1.创建压缩文件" << endl;
		cout << "2.解压缩文件" << endl;
		cout << "3.退出" << endl;
		printf("====================================\n");
		printf("请输入要执行的步骤:\n");
		char a = '0';
		cin >> a;
		switch (a) {
			case '1': {
				cout << "创建压缩文件" << endl;
				FileCompress file;
				cout << "输入需要压缩文件的文件名" << endl;
				string filename_Compress_s;
				cin >> filename_Compress_s;
				file.Compress(filename_Compress_s);
				break;
			}
			case '2': {
				cout << "解压缩文件" << endl;
				FileCompress file;
				cout << "输入需要解压缩文件的文件名" << endl;
				string filename_UnCompress_s;
				cin >> filename_UnCompress_s;
				file.UnCompress(filename_UnCompress_s);
				break;
			}
			case '3': {
				cout << "\n成功退出\n" << endl;
				return;
			}
			default: {
				cout << "\n========Error=======" << endl;
				cout << "输入错误请重新输入\n" << endl;
				break;
			}
		}
		cin.ignore(100, '\n');//把回车（包扩回车）之前的所以字符从输入缓冲流中清除出去
	}
}
