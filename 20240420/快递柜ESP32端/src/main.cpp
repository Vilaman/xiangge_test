#include <Arduino.h>
#include <PubSubClient.h>
#include <lvgl.h>
#include "UI/Showpage.h"
#include "FUNC/Display.h"
#include "FUNC/MQTTConnect.h"

extern PubSubClient client;

void setup() 
{
  Serial2.begin(115200);
  //屏幕初始化
  Display_init();
  //MQTT初始化
  MQTT_Connect_Init();
   //页面初始化
  LV_ShowPage_Init();
  /* Create simple label */
  Show_master_page();

}

void loop() {

  // 检查客户端是否连接，如果没有连接则重新连接
  if (!client.connected())
  {
    reconnect();
  }
  // 处理客户端的循环操作
  client.loop();
  lv_timer_handler(); /* 让 GUI 执行其任务 */
  delay(5);           /* 延迟 5 毫秒 */
}

