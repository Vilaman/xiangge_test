#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>

#define MAXLINE 80
#define SERV_PORT 8000

int main(int argc,char*argv[])
{
	struct sockaddr_in servaddr;
	char buf[MAXLINE];
	int sockfd,n;
	char *str;
	if(argc!=2)
	{
		fputs,("usage:./client message\n",stderr);
		exit(1);
	}
	str=argv[1];

	sockfd=socket(AF_INET,SOCK_STREAM,0);

	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	inet_pton(AF_INET,"192.168.80.130",&servaddr.sin_addr);
	servaddr.sin_port=htons(SERV_PORT);

	connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));

	write(sockfd,str,strlen(str));

	n=read(sockfd,buf,MAXLINE);
	printf("Response from server:\n");
	write(STDOUT_FILENO,buf,n);
	close(sockfd);
	return 0;
}
