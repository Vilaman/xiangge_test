#include "comServer.h"

void msSleep(unsigned int microSecond) {
	std::this_thread::sleep_for(std::chrono::milliseconds(microSecond));
}

// ������������
int rs485SetOpt(int Speed, int Bits, char Event, int Stop, int Rs485Fd) {
	// ��ȡ�ն�����
	struct termios options;
	
	if (tcgetattr(Rs485Fd, &options) != 0) {
		printf("rs485 Set Opt tcgetattr is error...");
		return -1;
	}
	// ���ÿ���ģʽ
	bzero(&options, sizeof(options));
	// ��֤����ռ�ô��� && ��֤������ԴӴ����ж�ȡ����
	options.c_cflag |= CLOCAL | CREAD;
	// ����������־λ
	options.c_cflag &= ~CSIZE;
	// ��������λ
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

	// ����У��λ
	switch (Event) {
	case 'n':                                               // ����żУ��λ
	case 'N': {
		options.c_cflag &= ~PARENB;                         // PARENB��������żλ��ִ����żУ��
		options.c_iflag &= ~INPCK;                          // INPCK��ʹ��żУ��������
	}
			break;
	case 'o':                                               // ������У��
	case 'O': {
		options.c_cflag |= PARENB;                          // PARENB��������żλ��ִ����żУ��
		options.c_cflag |= PARODD;                          // PARODD����������Ϊ��У��,����ΪżУ��
		options.c_iflag |= INPCK;                           // INPCK��ʹ��żУ�������� ISTRIP
		options.c_iflag |= ISTRIP;                          // ISTRIP������������Ч�������ֱ�����7���ֽڣ�������ȫ��8λ
	}
			break;
	case 'e':                                               // ����żУ��
	case 'E': {
		options.c_iflag |= INPCK;                           // INPCK��ʹ��żУ��������
		options.c_iflag |= ISTRIP;                          // ISTRIP������������Ч�������ֱ�����7���ֽڣ�������ȫ��8λ
		options.c_cflag |= PARENB;                          // PARENB��������żλ��ִ����żУ��
		options.c_cflag &= ~PARODD;                         // PARODD����������Ϊ��У��,����ΪżУ��
	}
			break;
	case 'S':                                               // ��Ϊ�ո�, ��ֹͣλΪ2λ
	case 's': {
		options.c_cflag &= ~PARENB;                         // PARENB��������żλ��ִ����żУ��
		options.c_cflag &= ~CSTOPB;                         // CSTOPB��ʹ����λֹͣλ
	}
			break;
	default: {
		printf("rs485SetOpt Event Unsupported parity");
		return -1;
	}
	}

	// ����������������ʣ����߱���һ��
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
	break; // ����� break
	case 19200:
	{
		cfsetispeed(&options, B19200);
		cfsetospeed(&options, B19200);
	}
	break; // ����� break
	case 38400:
	{
		cfsetispeed(&options, B38400);
		cfsetospeed(&options, B38400);
	}
	break; // ����� break
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

	// ���õȴ�ʱ�����С�����ַ� 
	options.c_cc[VTIME] = 0;                    // ������select������
	options.c_cc[VMIN] = 0;                     // ���ٶ�ȡһ���ַ�
	// ����������������ֻ�������ݣ����ǲ����ж�����
	tcflush(Rs485Fd, TCIFLUSH);
	// ��������
	if ((tcsetattr(Rs485Fd, TCSANOW, &options)) != 0) {
		printf("rs485SetOpt com set tcsetattr is error...");
		return -1;
	}
	return 0;
}

// �򿪴���
int rs485Open(const char* DevName) {
	int fd = -1;
	fd = open(DevName, O_RDWR | O_NOCTTY | O_NDELAY);         //| O_NOCTTY | O_NDELAY
	if (fd <= 0) {
		printf("rs485Open Can't Open RS485 Port DevName=[%s]", DevName);
		return -1;
	}
	return fd;
}

// ���ڳ�ʼ��
int rs485Init(const char* DevName, int Speed, int Bits, char Event, int Stop) {
	//step 1 rs485��˫���Ķ�д���Ƶ�GPIO��ʼ��
	SysGpio::SysGpio_Init(UART_CTRL_NUM);								//gpio �ɲ�����ʼ��
	SysGpio::SysGpio_SetDirection(UART_CTRL_NUM, SYS_GPIO_DIR_OUTPUT);	//gpio ��������
	SysGpio::SysGpio_SetValue(UART_CTRL_NUM, SYS_GPIO_VAL_L);			//��ʼ��ʱ���߼���Ӧ����Ϊ�ɽ���
	printf("Gpio passed!!!\n");
	// step 2 �򿪴���
	int Rs485Fd = rs485Open(DevName);
	printf("Rs485Fd=[%d]\n", Rs485Fd);
	if (Rs485Fd <= 0) {
		printf("rs485Init open fd failed DevName=[%s]\n", DevName);
		return -1;
	}
	printf("rs485Open passed DevName=[%s],Rs485Fd=[%d]\n", DevName, Rs485Fd);
	// step 3 ���ô���
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

// ������
int rs485Read(char* RecvBuf, int* RecvLen, int Rs485Fd) {
	if ((Rs485Fd <= 0) || (RecvBuf == NULL) || (RecvLen == NULL) || (*RecvLen <= 0)) {
		printf("rs485Read is error... m_nRs485Fd=[%d]<=0, RecvBuf=[%p], RecvLen=[%p], *RecvLen=[%d]<=0", Rs485Fd, RecvBuf, RecvLen, (RecvLen ? *RecvLen : -1));
		return -1;
	}

	// ��ʼ���ļ���������
	fd_set rset;
	FD_ZERO(&rset);
	FD_SET(Rs485Fd, &rset);

	// ���ó�ʱ
	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 30000;  // 30 milliseconds

	// �ȴ����ݿɶ�
	int nRet = select(Rs485Fd + 1, &rset, NULL, NULL, &timeout);
	if (nRet < 0) {
		printf("rs485Read select error... errno=[%d] %s", errno, strerror(errno));
		return -1;
	}
	else if (nRet == 0) {
		// ��ʱ���޿ɶ�����
		return 0;
	}
	else {
		// �ȴ�����ʱ��ȷ��������ȫ�������ӳٿ��ܸ��ݾ���Ӧ�ú��豸���ܵ���
		msSleep(100);

		// ��ȡ����
		int nRead = read(Rs485Fd, RecvBuf, *RecvLen);
		if (nRead < 0) {
			printf("rs485Read read error... nRead=[%d], errno=[%d] %s", nRead, errno, strerror(errno));
			return -1;  // ��ʾ���󲢷���-1
		}
		else if (nRead == 0) {
			return 0;
		}
		else {
			// �ɹ���ȡ����
			*RecvLen = nRead;  // ����ʵ�ʶ�ȡ���ֽ���
		}
		return nRead;
	}
}

// ������
int rs485Send(const char* SendBuf, int SendLen,int Rs485Fd) {
	int Ret = -1;
	Ret = SysGpio::SysGpio_SetValue(UART_CTRL_NUM, SYS_GPIO_VAL_H); //���߷���
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
	tcdrain(Rs485Fd);//tcdrain���� �ȴ�ֱ������д��  ���õĶ��������������䡣
	//tcflush���� ����Ҫд�� ���õĶ��󣬵�����δ��������ݣ������յ�������δ��ȡ�����ݣ�ȡ���� queue selector��ֵ
	tcflush(Rs485Fd, TCIOFLUSH);//TCIOFLUSH ͬʱˢ���յ������ݵ��ǲ���������ˢ��д������ݵ��ǲ�����

	Ret = SysGpio::SysGpio_SetValue(UART_CTRL_NUM, SYS_GPIO_VAL_L); //���ͽ���
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