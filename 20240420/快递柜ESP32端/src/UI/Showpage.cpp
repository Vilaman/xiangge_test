#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include "TAMC_GT911.h"
#include "UI/UserGet.h"
#include "UI/CourierInterface.h"
#include "../FUNC/Display.h"



/*LVGL初始化*/
// 定义屏幕的宽度和高度
static const uint16_t screenWidth  = TOUCH_WIDTH;
static const uint16_t screenHeight = TOUCH_HEIGHT;


extern TAMC_GT911 tp;
extern TFT_eSPI tft;
// 定义用于显示缓冲区的结构体
static lv_disp_draw_buf_t draw_buf;
// 定义用于存储颜色数据的缓冲区
static lv_color_t buf[ screenWidth * screenHeight / 10 ];

/* Display flushing */
void my_disp_flush( lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p )
{
    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );

    tft.startWrite();
    tft.setAddrWindow( area->x1, area->y1, w, h );
    tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
    tft.endWrite();

    lv_disp_flush_ready( disp_drv );
}

/*Read the touchpad*/
 void my_touchpad_read( lv_indev_drv_t * indev_drv, lv_indev_data_t * data )
{
//获取触摸的数据
  tp.read();
  //如果用户触摸了屏幕
  if (tp.isTouched){
  data->state = LV_INDEV_STATE_PR;
    for (int i=0; i<tp.touches; i++){
        data->point.x = tp.points[i].x;
        data->point.y = tp.points[i].y;
    }
  }
  else
  {
    data->state = LV_INDEV_STATE_REL;
  }
 }


// 定义显示屏的驱动程序
void LV_ShowPage_Init(void)
{
      //LVGL初始化
  lv_init();
  lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * screenHeight / 10 );

  /*Initialize the display*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv); // 初始化显示驱动
  /*Change the following line to your display resolution*/
  disp_drv.hor_res = screenWidth; // 设置水平分辨率
  disp_drv.ver_res = screenHeight; // 设置垂直分辨率
  disp_drv.flush_cb = my_disp_flush; // 设置刷新回调函数
  disp_drv.draw_buf = &draw_buf; // 设置绘图缓冲区
  lv_disp_drv_register(&disp_drv); // 注册显示驱动

  /*Initialize the (dummy) input device driver*/
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv); // 初始化输入设备驱动
  indev_drv.type = LV_INDEV_TYPE_POINTER; // 设置输入设备类型为指针
  indev_drv.read_cb = my_touchpad_read; // 设置读取回调函数
  lv_indev_drv_register(&indev_drv); // 注册输入设备驱动
}



void Show_master_page(void) // 创建两个按钮的示例函数
{
   /*Create a Tab view object*/
    lv_obj_t * tabview;
    tabview = lv_tabview_create(lv_scr_act(), LV_DIR_LEFT, 80);

    lv_obj_set_style_bg_color(tabview, lv_palette_lighten(LV_PALETTE_BLUE, 2), 0);

    lv_obj_t * tab_btns = lv_tabview_get_tab_btns(tabview);
    lv_obj_set_style_bg_color(tab_btns, lv_palette_darken(LV_PALETTE_DEEP_ORANGE, 3), 0);
    lv_obj_set_style_text_color(tab_btns, lv_palette_lighten(LV_PALETTE_GREY, 5), 0);
    lv_obj_set_style_border_side(tab_btns, LV_BORDER_SIDE_RIGHT, LV_PART_ITEMS | LV_STATE_CHECKED);

    /*Add 3 tabs (the tabs are page (lv_page) and can be scrolled*/
    lv_obj_t * tab1 = lv_tabview_add_tab(tabview, "取件");
    lv_obj_t * tab2 = lv_tabview_add_tab(tabview, "放件");
    // //定义字体
    // LV_FONT_DECLARE(XINGKAI_20);
    // //设置字体
    // lv_obj_set_style_text_font(tab1, &XINGKAI_20, 0);
    // lv_obj_set_style_text_font(tab2, &XINGKAI_20, 0);


    lv_obj_set_style_bg_color(tab2, lv_palette_lighten(LV_PALETTE_AMBER, 3), 0);
    lv_obj_set_style_bg_opa(tab2, LV_OPA_COVER, 0);

    /*Add content to the tabs*/
    UserGet(tab1);
    CourierInterface_Send(tab2);

    lv_obj_clear_flag(lv_tabview_get_content(tabview), LV_OBJ_FLAG_SCROLLABLE);
}

