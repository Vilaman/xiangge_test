/*********************main.c******************************/

#include "main.h"

void main(void)
{

    pthread_create(&tid_canopen_can, NULL, (void *)CANopen_CAN_Task, NULL);     //创建canopen接收线程
    pthread_create(&tid_canopen_timer, NULL, (void *)CANopen_Timer_Task, NULL); //创建canopen定时器线程

    sleep(1);
    unsigned char nodeID = 0x00; //节点ID
    setNodeId(&Master_Data, nodeID);
    setState(&Master_Data, Initialisation); //节点初始化
    setState(&Master_Data, Operational);
    while (1)
    {
        printf("hello \r\n");
        sleep(1);
    }
}
