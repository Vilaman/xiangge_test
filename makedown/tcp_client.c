#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#define CLI_IP "192.168.0.184"
#define CLI_PORT 8888

int main(int argc, char* argv[]){
    if(argc != 3){
        printf("请输入正确的参数\n");
        exit(1);
    }
    int cfd = socket(AF_INET, SOCK_STREAM, 0);
    if(cfd == -1){
        perror("socket error");
        exit(1);
    }
    struct sockaddr_in cliaddr, seraddr;
    /*
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(CLI_PORT);
    inet_pton(AF_INET, CLI_IP, &cliaddr.sin_addr.s_addr);
    int bret = bind(cfd, (struct sockaddr*)&cliaddr, sizeof(cliaddr));
    if(bret == -1){
        perror("bind error");
        exit(1);
    }*/
    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &seraddr.sin_addr.s_addr);
    int cret = connect(cfd, (struct sockaddr*)&seraddr, sizeof(seraddr));
    if(cret == -1){
        perror("connect error");
        exit(1);
    }
    //connect函数返回代表客户端和服务器完成三次握手
    char buf[1024];
    while(1){
        int rret = read(0, buf, sizeof(buf));
        write(cfd, buf, rret);
        rret = read(cfd, buf, sizeof(buf));
        write(1, buf, rret);
    }
    return 0;
}