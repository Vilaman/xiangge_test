#pragma once
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <thread>
#include <chrono>
#include <sys/select.h>
#include <sys/types.h>
#include <time.h>
#include <termios.h>
#include <iomanip>
#include <cctype>
#include "SysGpio.h"

#define RS485_FD  "/dev/ttyS1"
#define __STDCALL
typedef void (__STDCALL* ComRs485MsgCallBack)(int MsgType, char* MsgData, int MsgLen, void* UserData);

class comServer {
public:
	static comServer& getInstance() {
		static comServer instance;
		return instance;
	}
	int comRs485Init(int Speed);
	int comRs485DeInit();
	int sendComRs485Msg(void* SendData, unsigned int SendLen);
	void setRs485MsgCallBack(ComRs485MsgCallBack CallBackFunc, void* UserData);
	void loop();
private:
	comServer();
	~comServer();
	comServer(const comServer&) = delete;
	comServer& operator=(const comServer&) = delete;

	void processRs485Msg();
	void processRs485MsgCallBack(int MsgType, char* MsgData, int DataLen);

private:
	ComRs485MsgCallBack Rs485MsgCallBackFunc;
	void* Rs485UserData;
	int Rs485Fd;
};
