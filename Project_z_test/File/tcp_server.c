#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SER_PORT 8000
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len;
    char buffer[BUFFER_SIZE];
    char client_ip[INET_ADDRSTRLEN];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("套接字创建失败");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SER_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("绑定失败");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 64) < 0) {
        perror("监听失败");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("服务器正在监听端口:%d\n", SER_PORT);

    while (1) {
        client_addr_len = sizeof(client_addr);
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len);
        if (client_fd < 0) {
            perror("accept失败");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        printf("接受的连接来自 %s:%d\n", client_ip, ntohs(client_addr.sin_port));

        while (1) {
            memset(buffer, 0, BUFFER_SIZE);
            ssize_t read_bytes = read(client_fd, buffer, BUFFER_SIZE - 1);
            if (read_bytes <= 0) {
                if (read_bytes < 0) {
                    perror("read failed");
                }
                else {
                    printf("客户端关闭了连接\n");
                }
                break;
            }
            write(client_fd, buffer, read_bytes);
            printf("接收并回显: %s", buffer);
        }
        close(client_fd);
    }
    close(server_fd);
    return 0;
}
