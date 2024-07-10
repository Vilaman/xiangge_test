#include "SysGpio.h"

SysGpio::SysGpio() {}
SysGpio::~SysGpio() {}

/*==================================================================
* Function      : SysGpio_Init
* Description   : 初始化gpio
* Input Para    : n对应io的引脚号
* Output Para   : 
* Return Value  : 
==================================================================*/
void SysGpio::SysGpio_Init(int GpioNum) {
	FILE* fp = fopen("/sys/class/gpio/export", "w");
	if (fp == NULL) {
		printf("SysGpio_Init fopen failed\n");
		return;
	}
	else {
		fprintf(fp, "%d", GpioNum);
		printf("SysGpio_Init export open success\n");
	}
	fclose(fp);
}

/*==================================================================
* Function      : SysGpio_Deinit
* Description   : 去初始化gpio
* Input Para    : n对应io的引脚号
* Output Para   : 
* Return Value  : 
==================================================================*/
void SysGpio::SysGpio_Deinit(int GpioNumn) {
	FILE* fp = fopen("/sys/class/gpio/unexport", "w");
	if (fp == NULL) {
		printf("SysGpio_Deinit unexport open failed\n");
		return;
	}
	else {
		fprintf(fp, "%d", GpioNumn);
		printf("SysGpio_Deinit unexport open success\n");
	}
	fclose(fp);
}

/*==================================================================
* Function      : SysGpio_SetDirection
* Description   : 设置GPIO方向
* Input Para    : n对应io的引脚号，direction为"in"或者"out"
* Output Para   : 
* Return Value  : 
==================================================================*/
void SysGpio::SysGpio_SetDirection(int GpioNumn, const char* Direction) {
	char path[64] = { 0 };
	sprintf(path, "/sys/class/gpio/gpio%d/direction", GpioNumn);
	FILE* fp = fopen(path, "w");
	if (fp == NULL) {
		printf("SysGpio_SetDirection direction open failed\n");
		return;
	}
	else {
		fprintf(fp, "%s", Direction);
		printf("SysGpio_SetDirection direction open success\n");
	}
	fclose(fp);
}

/*==================================================================
* Function      : SysGpio_GetValue
* Description   : 获取GPIO的值
* Input Para    : n对应io的引脚号
* Output Para   : 返回0或1
* Return Value  : 
==================================================================*/
int SysGpio::SysGpio_GetValue(int GpioNumn) {
	char path[64];
	char value_str[3] = { 0 };
	sprintf(path, "/sys/class/gpio/gpio%d/value", GpioNumn);
	int fd = open(path, O_RDONLY);
	if (fd < 0) {
		printf("SysGpio_GetValue Failed to open gpio value for reading!\n");
		return -1;
	}
	if (read(fd, value_str, 3) < 0) {
		printf("SysGpio_GetValue Failed to read value!\n");
		return -1;
	}
	close(fd);
	return (atoi(value_str));
}

/*==================================================================
* Function      : SysGpio_SetValue
* Description   : 设置GPIO的值
* Input Para    : n对应io的引脚号，value对应电平0,1
* Output Para   : 
* Return Value  : 
==================================================================*/
int SysGpio::SysGpio_SetValue(int GpioNumn,const char* Value) {
	char path[64];
	sprintf(path, "/sys/class/gpio/gpio%d/value", GpioNumn);
	int fd = open(path, O_WRONLY);
	if (fd < 0) {
		printf("SysGpio_SetValue Failed to open gpio value for reading!\n");
		return -1;
	}
	if (write(fd, Value, sizeof(Value)) < 0) {
		printf("SysGpio_SetValue Failed to write value!\n");
		return -1;
	}
	close(fd);
	return 0;
}
