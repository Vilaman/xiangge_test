#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

/* GPIO参数定义 */
#define SYS_GPIO_DIR_INPUT  "in"
#define SYS_GPIO_DIR_OUTPUT "out"
#define SYS_GPIO_VAL_H      "1"     // 对于RS485而言 处于高电平时发送状态，想要接受需要置低位
#define SYS_GPIO_VAL_L      "0"     // 对于RS485而言 处于低电平时接受状态，想要发送需要置高位
#define SYS_GPIO_OPEN_LED   SYS_GPIO_VAL_L
#define SYS_GPIO_CLOSE_LED  SYS_GPIO_VAL_H
#define SYS_DAY_STATUS      (0)
#define SYS_NIGHT_STATUS    (1)
#define UART_CTRL_NUM (89)

class SysGpio
{
public:
    //Gpioc构造函数和析构函数
    SysGpio();
    ~SysGpio();
    //Gpio初始化
    static void SysGpio_Init(int GpioNum);
    //Gpio去初始化
    static void SysGpio_Deinit(int GpioNum);
    //Gpio设置方向
    static void SysGpio_SetDirection(int GpioNum, const char *Direction);
    //Gpio获取值
    static int SysGpio_GetValue(int GpioNum);
    //Gpio设置值
    static int SysGpio_SetValue(int GpioNum, const char *Value);
};
