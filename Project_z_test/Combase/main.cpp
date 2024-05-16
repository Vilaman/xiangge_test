#include "comServer.h"
#include <iostream>
#include <unistd.h>
#include <string.h>
comServer a;

// 485接收消息回调
void comRs485MsgCallBack(int nMsgType, char* pMsgData, int msgLen, void* pUserData) {
	std::cout << "=====================" << std::endl;
	std::cout << "nmsgType = " << nMsgType << std::endl;
	std::cout << "msgLen = " << msgLen << std::endl;
	if (pMsgData != NULL) {
		std::cout << "pMsgData = " << pMsgData << std::endl;
	}

	if (pUserData != NULL) {
		std::cout << "pUserData = " << pUserData << std::endl;
	}
	std::cout << "=====================" << std::endl;
	// 发送485消息
	char szAck[] = { 0x30, 0x31, 0x32,0x30, 0x31, 0x32,0x30, 0x31,0x32,0x30,0x30, 0x31, 0x32,0x30, 0x31, 0x32,0x30, 0x31,0x32,0x30 };
	printf(" send ret %d\n ", a.sendComRs485Msg(szAck, sizeof(szAck)));
}

int main(){
	a.comRs485Init(115200);
	a.setRs485MsgCallBack(comRs485MsgCallBack, NULL);
	while (1){
		a.loop();
	}
	a.comRs485DeInit();
	return 0;
}
