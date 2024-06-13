#include "comServer.h"

void msSleep(unsigned int microSecond) {
	std::this_thread::sleep_for(std::chrono::milliseconds(microSecond));
}

// 串口属性设置
int rs485SetOpt(int Speed, int Bits, char Event, int Stop, int Rs485Fd) {
	// 获取终端属性
	struct termios options;
	
	if (tcgetattr(Rs485Fd, &options) != 0) {
		printf("rs485 Set Opt tcgetattr is error...");
		return -1;
	}
	// 设置控制模式
	bzero(&options, sizeof(options));
	// 保证程序不占用串口 && 保证程序可以从串口中读取数据
	options.c_cflag |= CLOCAL | CREAD;
	// 屏蔽其它标志位
	options.c_cflag &= ~CSIZE;
	// 设置数据位
	switch (Bits) {
	case 7: {
		options.c_cflag |= CS7;
	}
		  break;
	case 8: {
		options.c_cflag |= CS8;
	}
		  break;
	default: {
		printf("rs485SetOpt Bits Unsupported data size");
		return -1;
	}
	}

	// 设置校验位
	switch (Event) {
	case 'n':                                               // 无奇偶校验位
	case 'N': {
		options.c_cflag &= ~PARENB;                         // PARENB：产生奇偶位，执行奇偶校验
		options.c_iflag &= ~INPCK;                          // INPCK：使奇偶校验起作用
	}
			break;
	case 'o':                                               // 设置奇校验
	case 'O': {
		options.c_cflag |= PARENB;                          // PARENB：产生奇偶位，执行奇偶校验
		options.c_cflag |= PARODD;                          // PARODD：若设置则为奇校验,否则为偶校验
		options.c_iflag |= INPCK;                           // INPCK：使奇偶校验起作用 ISTRIP
		options.c_iflag |= ISTRIP;                          // ISTRIP：若设置则有效输入数字被剥离7个字节，否则保留全部8位
	}
			break;
	case 'e':                                               // 设置偶校验
	case 'E': {
		options.c_iflag |= INPCK;                           // INPCK：使奇偶校验起作用
		options.c_iflag |= ISTRIP;                          // ISTRIP：若设置则有效输入数字被剥离7个字节，否则保留全部8位
		options.c_cflag |= PARENB;                          // PARENB：产生奇偶位，执行奇偶校验
		options.c_cflag &= ~PARODD;                         // PARODD：若设置则为奇校验,否则为偶校验
	}
			break;
	case 'S':                                               // 设为空格, 即停止位为2位
	case 's': {
		options.c_cflag &= ~PARENB;                         // PARENB：产生奇偶位，执行奇偶校验
		options.c_cflag &= ~CSTOPB;                         // CSTOPB：使用两位停止位
	}
			break;
	default: {
		printf("rs485SetOpt Event Unsupported parity");
		return -1;
	}
	}

	// 设置输入输出波特率，两者保持一致
	switch (Speed) {
	case 2400:
	{
		cfsetispeed(&options, B2400);
		cfsetospeed(&options, B2400);
	}
	break;
	case 4800:
	{
		cfsetispeed(&options, B4800);
		cfsetospeed(&options, B4800);
	}
	break;
	case 9600:
	{
		cfsetispeed(&options, B9600);
		cfsetospeed(&options, B9600);
	}
	break; // 添加了 break
	case 19200:
	{
		cfsetispeed(&options, B19200);
		cfsetospeed(&options, B19200);
	}
	break; // 添加了 break
	case 38400:
	{
		cfsetispeed(&options, B38400);
		cfsetospeed(&options, B38400);
	}
	break; // 添加了 break
	case 57600:
	{
		cfsetispeed(&options, B57600);
		cfsetospeed(&options, B57600);
	}
	break;
	case 115200:
	{
		cfsetispeed(&options, B115200);
		cfsetospeed(&options, B115200);
	}
	break;
	case 460800:
	{
		cfsetispeed(&options, B460800);
		cfsetospeed(&options, B460800);
	}
	break;
	default:
	{
		cfsetispeed(&options, B9600);
		cfsetospeed(&options, B9600);
	}
	break;
	}

	if (Stop == 1) {
		options.c_cflag &= ~CSTOPB;
	}
	else if (Stop == 2) {
		options.c_cflag |= CSTOPB;
	}

	// 设置等待时间和最小接受字符 
	options.c_cc[VTIME] = 0;                    // 可以在select中设置
	options.c_cc[VMIN] = 0;                     // 最少读取一个字符
	// 如果发生数据溢出，只接受数据，但是不进行读操作
	tcflush(Rs485Fd, TCIFLUSH);
	// 激活配置
	if ((tcsetattr(Rs485Fd, TCSANOW, &options)) != 0) {
		printf("rs485SetOpt com set tcsetattr is error...");
		return -1;
	}
	return 0;
}

// 打开串口
int rs485Open(const char* DevName) {
	int fd = -1;
	fd = open(DevName, O_RDWR | O_NOCTTY | O_NDELAY);         //| O_NOCTTY | O_NDELAY
	if (fd <= 0) {
		printf("rs485Open Can't Open RS485 Port DevName=[%s]", DevName);
		return -1;
	}
	return fd;
}

// 串口初始化
int rs485Init(const char* DevName, int Speed, int Bits, char Event, int Stop) {
	//step 1 rs485半双工的读写控制的GPIO初始化
	SysGpio::SysGpio_Init(UART_CTRL_NUM);								//gpio 可操作初始化
	SysGpio::SysGpio_SetDirection(UART_CTRL_NUM, SYS_GPIO_DIR_OUTPUT);	//gpio 方向设置
	SysGpio::SysGpio_SetValue(UART_CTRL_NUM, SYS_GPIO_VAL_L);			//初始化时，逻辑上应设置为可接受
	printf("Gpio passed!!!\n");
	// step 2 打开串口
	int Rs485Fd = rs485Open(DevName);
	printf("Rs485Fd=[%d]\n", Rs485Fd);
	if (Rs485Fd <= 0) {
		printf("rs485Init open fd failed DevName=[%s]\n", DevName);
		return -1;
	}
	printf("rs485Open passed DevName=[%s],Rs485Fd=[%d]\n", DevName, Rs485Fd);
	// step 3 配置串口
	int Ret = rs485SetOpt(Speed, Bits, Event, Stop, Rs485Fd);
	if (Ret < 0) {
		close(Rs485Fd);
		Rs485Fd = -1;
		printf("rs485Init set opt failed Speed=[%d] Bits=[%d] Event=[%d] Stop=[%d]\n", Speed, Bits, Event, Stop);
		return -1;
	}
	printf("rs485SetOpt passed Ret=[%d]\n", Ret);
	return Rs485Fd;
}

int rs485DeInit(int Rs485Fd) {
	SysGpio::SysGpio_Deinit(UART_CTRL_NUM);
	close(Rs485Fd);
	return 0;
}

// 读数据
int rs485Read(char* RecvBuf, int* RecvLen, int Rs485Fd) {
	if ((Rs485Fd <= 0) || (RecvBuf == NULL) || (RecvLen == NULL) || (*RecvLen <= 0)) {
		printf("rs485Read is error... m_nRs485Fd=[%d]<=0, RecvBuf=[%p], RecvLen=[%p], *RecvLen=[%d]<=0", Rs485Fd, RecvBuf, RecvLen, (RecvLen ? *RecvLen : -1));
		return -1;
	}

	// 初始化文件描述符集
	fd_set rset;
	FD_ZERO(&rset);
	FD_SET(Rs485Fd, &rset);

	// 设置超时
	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 30000;  // 30 milliseconds

	// 等待数据可读
	int nRet = select(Rs485Fd + 1, &rset, NULL, NULL, &timeout);
	if (nRet < 0) {
		printf("rs485Read select error... errno=[%d] %s", errno, strerror(errno));
		return -1;
	}
	else if (nRet == 0) {
		// 超时但无可读数据
		return 0;
	}
	else {
		// 等待短暂时间确保数据完全到达，这个延迟可能根据具体应用和设备性能调整
		msSleep(100);

		// 读取数据
		int nRead = read(Rs485Fd, RecvBuf, *RecvLen);
		if (nRead < 0) {
			printf("rs485Read read error... nRead=[%d], errno=[%d] %s", nRead, errno, strerror(errno));
			return -1;  // 显示错误并返回-1
		}
		else if (nRead == 0) {
			return 0;
		}
		else {
			// 成功读取数据
			*RecvLen = nRead;  // 更新实际读取的字节数
		}
		return nRead;
	}
}

// 发数据
int rs485Send(const char* SendBuf, int SendLen,int Rs485Fd) {
	int Ret = -1;
	Ret = SysGpio::SysGpio_SetValue(UART_CTRL_NUM, SYS_GPIO_VAL_H); //拉高发送
	printf("SysGpio_SetValue=[%d]\n", Ret);
	Ret = SysGpio::SysGpio_GetValue(UART_CTRL_NUM);
	printf("SysGpio_GetValue=[%d]\n", Ret);
	Ret = write(Rs485Fd, SendBuf, SendLen);
	printf("rs485Send send len=[%d]\n", Ret);
	if (Ret != SendLen) {
		printf("rs485Send write is error... Ret != SendLen Ret=[%d] SendLen=[%d]\n", Ret, SendLen);
		Ret = SysGpio::SysGpio_SetValue(UART_CTRL_NUM, SYS_GPIO_VAL_L);
		return -1;
	}

	msSleep(10);
	tcdrain(Rs485Fd);//tcdrain函数 等待直到所有写入  引用的对象的输出都被传输。
	//tcflush函数 丢弃要写入 引用的对象，但是尚未传输的数据，或者收到但是尚未读取的数据，取决于 queue selector的值
	tcflush(Rs485Fd, TCIOFLUSH);//TCIOFLUSH 同时刷新收到的数据但是不读，并且刷新写入的数据但是不传送

	Ret = SysGpio::SysGpio_SetValue(UART_CTRL_NUM, SYS_GPIO_VAL_L); //拉低接收
	printf("SysGpio_SetValue=[%d]\n", Ret);
	Ret = SysGpio::SysGpio_GetValue(UART_CTRL_NUM);
	printf("SysGpio_GetValue=[%d]\n", Ret);
	return Ret;
}

int comServer::comRs485Init(int Speed) {
	int nRet = rs485Init(RS485_FD, Speed, 8, 'N', 1);
	if (nRet == -1) {
		printf("comRs485Init is failed\n");
		return -1;
	}
	comServer::Rs485Fd = nRet;
	printf("comRs485Init Rs485Fd=[%d]\n", nRet);
	return 0;
}

int comServer::comRs485DeInit() {
	rs485DeInit(comServer::Rs485Fd);
	Rs485MsgCallBackFunc = NULL;
	Rs485UserData = NULL;
	return 0;
}

comServer::comServer() : Rs485Fd(-1), Rs485MsgCallBackFunc(nullptr), Rs485UserData(nullptr) {}

comServer::~comServer() {
	comRs485DeInit();
}

int comServer::sendComRs485Msg(void* SendData, unsigned int SendLen) {
	int ret = rs485Send((char*)SendData, SendLen,comServer::Rs485Fd);
	if (ret == -1) {
		return -1;
	}
	return 0;
}

void comServer::setRs485MsgCallBack(ComRs485MsgCallBack CallBackFunc, void* UserData) {
	Rs485MsgCallBackFunc = CallBackFunc;
	Rs485UserData = UserData;
}

void comServer::processRs485MsgCallBack(int MsgType, char* MsgData, int DataLen) {
	if (NULL != Rs485MsgCallBackFunc) {
		Rs485MsgCallBackFunc(MsgType, MsgData, DataLen, Rs485UserData);
		printf("comServer::processRs485MsgCallBack is suc... msgType=[%d] dateLen=[%d]\n", MsgType, DataLen);
	}
	else {
		printf("comServer::processRs485MsgCallBack is error... Rs485MsgCallBackFunc=[NULL] msgType=[%d] dateLen=[%d]\n",MsgType, DataLen);
	}
}

void comServer::processRs485Msg() {
	int  nRecvLen = 1024;
	char szRecvBuf[1024];
	memset(szRecvBuf, 0, sizeof(szRecvBuf));
	int Read = rs485Read(szRecvBuf, &nRecvLen, comServer::Rs485Fd);
	if (Read > 0) {
		printf("processRs485Msg recv : \n");
		for (int i = 0; i < Read; i++) {
			auto isPrintable = [](char ch) -> bool {
				return std::isprint(static_cast<unsigned char>(ch)) != 0;
			};

			std::cout << "szRecvBuf[" << i << "] = " << std::hex << std::showbase << std::internal << std::setfill('0')
				<< std::setw(4) << static_cast<unsigned>(static_cast<unsigned char>(szRecvBuf[i]))
				<< std::dec << " (" << (isPrintable(szRecvBuf[i]) ? szRecvBuf[i] : '.') << ")" << std::endl;
		}
		int MsgId = *((int*)szRecvBuf);
		this->processRs485MsgCallBack(MsgId, szRecvBuf, Read);
	}
}

void comServer::loop() {
	processRs485Msg();
}