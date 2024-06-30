#include <Arduino.h>
#include <TFT_eSPI.h>
#include "TAMC_GT911.h"
#include "FUNC/Display.h"

/*TFT显示屏和触摸屏初始化*/
TFT_eSPI tft = TFT_eSPI(Width,Height);

TAMC_GT911 tp = TAMC_GT911(TOUCH_SDA, TOUCH_SCL, TOUCH_INT, TOUCH_RST, TOUCH_WIDTH, TOUCH_HEIGHT);


 void Display_init(void)
 {
  tft.begin();
  // 设置屏幕旋转角度
  tft.setRotation(3);
  // 填充屏幕颜色为黄色
  tft.fillScreen(TFT_YELLOW);
  // 初始化触摸屏
  tp.begin();
  // 设置触摸屏旋转角度
  tp.setRotation(ROTATION_LEFT);
 }