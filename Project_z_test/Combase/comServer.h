#pragma once
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <thread>
#include <chrono>
#include <sys/select.h>
#include <sys/types.h>
#include <time.h>
#include <termios.h> //POSIX终端控制定义 
#include "SysGpio.h"

#define RS485_FD  "/dev/ttyS1"
#define __STDCALL
typedef void (__STDCALL* ComRs485MsgCallBack)(int MsgType, char* MsgData, int MsgLen, void* UserData);

class comServer {
public:
	comServer();
	~comServer();
	int comRs485Init(int Speed);
	int comRs485DeInit();
	int sendComRs485Msg(void* SendData, unsigned int SendLen);
	void setRs485MsgCallBack(ComRs485MsgCallBack CallBackFunc, void* UserData);
	void processRs485Msg();
	void processRs485MsgCallBack(int MsgType, char* MsgData, int DataLen);
	void loop();
private:
	ComRs485MsgCallBack Rs485MsgCallBackFunc; // 回调接口
	void* Rs485UserData; // 私有数据
};

