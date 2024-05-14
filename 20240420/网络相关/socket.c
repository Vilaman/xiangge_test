/***********************************************************************************
# File Name:    socket.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年04月29日 星期一 15时09分29秒
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
#define PORT 8088

int main(){
/*
 * 1.调用socket()创建一个套接字，并指定ip地址和使用协议，得到一个套接字描述符
 * 2.调用connet()尝试连接一个服务器
 * 3.链接成功后使用read()或者write()进行接收服务器返回的数据或发送数据给服务器
 * 4.不需要链接之后调用close()关闭套接字，释放套接字所占用资源*/
	//1.定义一个套接字

	int client_fd=socket(AF_INET,SOCK_STREAM,0);

	if(client_fd<0){
		perror("socket()");
		exit(1);
	}
	//构建一个socket_in结构体，用于描述要连接服务器的基本信息：协议 端口号以及ip地址
	//在第三步链接需要将该结构体强转为socketaddr类型

	struct sockaddr_in server;

	//设置协议 端口号以及ip地址(本机地址127.0.0.1或localhost)
	//
	server.sin_family=AF_INET;//IPV4
	server.sin_port=htons(PORT);
	server.sin_addr.s_addr=inet_addr("127.0.0.1");//设置服务器ip

	if(connect(client_fd,(struct sockaddr *)&server,sizeof(server))==-1){
	perror("connect()");
	exit(1);
	}
	puts("链接服务区成功");

	//读写数据
	//写
	do{
	puts("请输入发送的数据");
	char data[50];
	scanf(" %s",data);
	int writecount=write(client_fd,data,sizeof(data));
	if(writecount>0){
	puts("发送数据成功");
	}
	
	//读取数据
	memset(data,0,sizeof(data));//初始化data数组

	int readcount=read(client_fd,data,sizeof(data)-1);
	if(readcount>0){
	data[readcount]='\0';
	puts(data);
	}else{
		puts("读取数据失败");
	}
	}while(1);
	close(client_fd);

    return 0;
}
