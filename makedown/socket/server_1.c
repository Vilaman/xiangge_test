#include <stdio.h>
#include <unistd.h> 
#include <fcntl.h>
#include <stdlib.h> 
#include <sys/wait.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#define SERPORT 8000

int main(int argc, char* argv[]) {
	//拥有一个socket
	int lfd=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in seraddr, cliaddr;
	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(SERPORT);
	seraddr.sin_addr.s_addr = 0;

	int ret = bind(lfd, (struct sockaddr*)&seraddr, sizeof(seraddr));
	if (ret < 0) {
		perror("bind error");
		return 0;
	}
	listen(lfd, 64);
	socklen_t len = sizeof(cliaddr);
	int cfd = accept(lfd, (struct sockaddr*)&cliaddr, &len);
	if (cfd<0) {
		perror("""accept error");
		return 0;
	}
	write(cfd,"hello world",11);
	while (1);
	return 0;
}                                                         
