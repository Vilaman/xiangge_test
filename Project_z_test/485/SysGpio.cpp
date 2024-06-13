#include "SysGpio.h"

SysGpio::SysGpio() {}
SysGpio::~SysGpio() {}

// 初始化gpio  GpioNum对应io的引脚号
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

// 去初始化gpio	GpioNum对应io的引脚号
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

// 设置GPIO方向	GpioNum对应io的引脚号	Direction为"in"或者"out"
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

// 获取GPIO的值    GpioNum对应io的引脚号
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

// 设置GPIO的值	GpioNum对应io的引脚号,Value对应电平0,1
int SysGpio::SysGpio_SetValue(int GpioNumn,const char* Value) {
	char path[64];
	sprintf(path, "/sys/class/gpio/gpio%d/value", GpioNumn);
	int fd = open(path, O_WRONLY);
	if (fd < 0) {
		printf("SysGpio_SetValue Failed to open gpio value for reading!\n");
		return -1;
	}
	if (write(fd, Value, sizeof(GpioNumn)) < 0) {
		printf("SysGpio_SetValue Failed to write value!\n");
		return -1;
	}
	close(fd);
	return 0;
}
