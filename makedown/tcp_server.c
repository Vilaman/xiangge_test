#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#define SER_IP "192.168.0.184"
#define SER_PORT 10001   

int main(int argc, char* argv[]){
    //创建一个套接字， AF_INET：IPv4地址，SOCK_STREAM：流式套接字， 0：使用TCP协议
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if(lfd == -1){
        perror("socket error");
        exit(1);
    }
    //一个描述地址结构（地址类型+IP+PORT）的变量
    struct sockaddr_in seraddr, cliaddr;
    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(SER_PORT);
    seraddr.sin_addr.s_addr = INADDR_ANY;//INADDR_ANY = 0
    //给socket绑定IP+端口，被绑定了IP和端口的socket才可以用于“网络通信”
    int bret = bind(lfd, (struct sockaddr*)&seraddr, sizeof(seraddr));
    if(bret == -1){
        perror("bind error");
        exit(1);
    }
    listen(lfd, 64);
    socklen_t len = sizeof(cliaddr);
    int cfd = accept(lfd, (struct sockaddr*)&cliaddr, &len);
    if(cfd == -1){
        perror("accept error");
        exit(1);
    }
    //IP：大端字节序的整型转成小端字节序的点分十进制字符串
    char dst[32];
    inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, dst, sizeof(dst));

    printf("clien IP = %s, PORT = %d\n", dst, ntohs(cliaddr.sin_port));
    char buf[1024];
    //回射服务器
    while(1){
        int rret = read(cfd, buf, sizeof(buf));
        if(rret == -1){
            perror("read error");
            exit(1);
        }
        else if(rret == 0){
            //read 返回0，表示对端断开连接
            printf("客户端断开连接\n");
            break;
        }
        else{
            write(cfd, buf, rret);
            write(1, buf, rret);
        }

    }
    return 0;
}