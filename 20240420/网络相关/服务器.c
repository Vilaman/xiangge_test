/***********************************************************************************
# File Name:    服务器.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年04月29日 星期一 16时00分40秒
 **********************************************************************************/

#include <stdio.h>
#include<string.h> 
#include<stdbool.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>
int main(){
	/*
	 * 1.调用socket()创建一个套接字，并指定ip地址和使用协议，得到一个套接字描述符
	 * 2.调用bind()绑定一个端口
	 * 3.调用listen()用于监听绑定的端口，可以处理任何发送给该端口的信息(比如客户端发来的，需要accept来处理)
	 * 4.需要手动调用accept来接受客户端的链接
	 * 5.第四步成功之后就可以使用read()和write()来完成数据的交互
	 * 6.不需要链接之后调用close()关闭套接字，释放套接字所占用的资源
	 */

	//1.创建套接字
	int server_fd=socket(AF_INET,SOCK_STREAM,0);
	if(server_fd<0){
	perror("socket()");
	exit(0);
	}
	//绑定端口
	struct sockaddr_in server;
	server.sin_family=AF_INET;
	server.sin_port=htons(8088);//设置端口
	server.sin_addr.s_addr=INADDR_ANY;//可以接受任意ip地址链接
	//绑定端口
	if(bind(server_fd,(struct sockaddr *)&server,sizeof(server))==-1){

		perror("bind()");
		exit(1);

	}

	//监听端口
	if(listen(server_fd,1)==-1){
		perror("listen()");
		exit(1);
	}

//接受链接
	int client_fd;//客户端套接字描述符，方便后续的数据交互
	struct sockaddr_in client;//定义结构体用于保存客户端连接信息
	socklen_t len=sizeof(client);//获取accept中的len
	puts("等待连接中....................");
	if((client_fd=accept(server_fd,(struct sockaddr *)&client,&len))==-1){

	perror("accept()");
	exit(1);
	}
	char *ip=inet_ntoa(client.sin_addr);//定义ip转换为我们能看懂的类型
	printf("接受来自客户端：%s的链接\n",ip);
	//读写数据
	
	int readcount,writecount;
	char data[100];
	do{
		memset(data,0,sizeof(data));//清空数组的内容
		readcount=read(client_fd,data,sizeof(data)-1);
		if(readcount>0){
			data[readcount]='\0';
			printf("来自客户端的数据是%s\n",data);
		}
	char str[100]="";
	sprintf(str,"服务器已接收数据：%s",data);
	writecount=write(client_fd,str,sizeof(str));
	if(writecount>0){
		puts("服务器发送数据成功");
	}
	}while(readcount>0&&writecount>0);

    return 0;
}
