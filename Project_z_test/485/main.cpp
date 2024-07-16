#include <iostream>
#include <unistd.h>
#include <vector>
#include <cstring>
#include "comServer.h"
//comServer& g_comServer = comServer::getInstance();

// 485接收消息回调函数
void comRs485MsgCallBack(int MsgType, char* MsgData, int msgLen, void* pUserData) {
    std::cout << "=====================" << std::endl;
    std::cout << "nMsgType = " << MsgType << std::endl;
    std::cout << "msgLen = " << msgLen << std::endl;
    if (MsgData != NULL) {
        std::cout << "pMsgData = " << std::string(MsgData, msgLen) << std::endl; //安全地打印消息内容
    }
    if (pUserData != NULL) {
        std::cout << "pUserData = " << pUserData << std::endl;
    }
    std::cout << "=====================" << std::endl;

    // 发送485消息
    std::vector<unsigned char> szAck = { 0x30, 0x31, 0x32, 0x30, 0x31, 0x32, 0x30, 0x31, 0x32, 0x30, 0x30, 0x31, 0x32, 0x30, 0x31, 0x32, 0x30, 0x31, 0x32, 0x30 };
    std::cout << "Send ret: " << comServer::getInstance()->sendComRs485Msg(szAck.data(), szAck.size()) << std::endl;
}

int main() {
    int fd = 115200;
    std::cout << "请输入所需要的波特率：";
    std::cin >> fd;
    if (comServer::getInstance()->comRs485Init(fd) != 0) {
        std::cerr << "Failed to initialize RS485 communication." << std::endl;
        return -1;
    }

    comServer::getInstance()->setRs485MsgCallBack(comRs485MsgCallBack, NULL);

    // 使用标记控制循环，以便优雅地退出
    bool running = true;
    while (running) {
        comServer::getInstance()->loop();
    }
    comServer::getInstance()->comRs485DeInit();
    return 0;
}