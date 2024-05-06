#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define BUFFER_SIZE 1024

int main(int argc, char* argv[]) {
	if (argc != 3) {
		printf("请输入正确的参数\n");
		exit(EXIT_FAILURE);
	}
	int client_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (client_fd < 0) {
		perror("套接字创建失败");
		exit(EXIT_FAILURE);
	}
	struct sockaddr_in server_addr, client_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(argv[2]));
	if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) <= 0) {
		printf("无效地址/不支持地址\n");
		exit(EXIT_FAILURE);
	}

	if ((connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr))) < 0) {
		perror("连接失败");
		exit(EXIT_FAILURE);
	}

	char buffer[BUFFER_SIZE];
	while (1) {
		memset(buffer, 0, BUFFER_SIZE);
		printf("输入信息: ");
		fflush(stdout);
		ssize_t read_ret = read(STDIN_FILENO, buffer, sizeof(buffer));
		if (read_ret < 0) {
			perror("读错误");
			break;
		}
		else if (read_ret == 0) {
			printf("EOF. 退出.\n");
			break;
		}
		if (write(client_fd, buffer, read_ret) < 0) {
			perror("写错误");
			break;
		}
	}
	close(client_fd);
	return 0;
}
