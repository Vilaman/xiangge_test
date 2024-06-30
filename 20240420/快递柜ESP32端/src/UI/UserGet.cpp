#include <Arduino.h>
#include <lvgl.h>
#include <PubSubClient.h>
#include "UI/CourierInterface.h"
#include "../FUNC/Claim_data.h"



extern PubSubClient client;

// 定义取件码数据结构
Claim_BoxData Claim_data[4]=
{
    false,"",1,"",
    false,"",2,"",
    false,"",3,"",
    false,"",4,"",
};

static lv_obj_t * mbox1;
//弹窗时间处理函数
static void event_cb(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_current_target(e);
    const char * txt = lv_msgbox_get_active_btn_text(obj);
    lv_msgbox_close(mbox1);
}

//弹窗提示
 static void Msgbox_Show(const char* title, const char* msg)
{
        static const char * btns[] = {"确定", ""};
        mbox1 = lv_msgbox_create(NULL, title, msg, btns, false);
        lv_obj_add_event_cb(mbox1, event_cb, LV_EVENT_VALUE_CHANGED, NULL);
        lv_obj_set_size(mbox1, 200, 120);                                                       /* 设置大小 */
        lv_obj_center(mbox1);                                                                   /* 设置位置 */
        lv_obj_set_style_border_width(mbox1, 0, LV_STATE_DEFAULT);                              /* 去除边框 */
        lv_obj_set_style_shadow_width(mbox1, 20, LV_STATE_DEFAULT);                             /* 设置阴影宽度 */
        lv_obj_set_style_shadow_color(mbox1, lv_color_hex(0xa9a9a9), LV_STATE_DEFAULT);         /* 设置阴影颜色 */
        lv_obj_set_style_pad_top(mbox1,18,LV_STATE_DEFAULT);                                    /* 设置顶部填充 */
        lv_obj_set_style_pad_left(mbox1,20,LV_STATE_DEFAULT); 
}

// 处理文本区域事件的静态函数
static void textarea_event_handler(lv_event_t * e)
{
    // 获取事件目标对象（文本区域）
    lv_obj_t * ta = lv_event_get_target(e);
    // 记录用户日志，显示当前文本区域的文本内容
    LV_LOG_USER("Enter was pressed. The current text is: %s", lv_textarea_get_text(ta));
}

// 处理按钮矩阵事件的静态函数
static void btnm_event_handler(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e); // 获取事件目标对象
    lv_obj_t * ta = (lv_obj_t *)lv_event_get_user_data(e); // 获取用户数据对象
    const char * txt = lv_btnmatrix_get_btn_text(obj, lv_btnmatrix_get_selected_btn(obj)); // 获取选中的按钮文本
    int Box_id=0;
    if(strcmp(txt, LV_SYMBOL_BACKSPACE) == 0) lv_textarea_del_char(ta); // 如果文本是退格键，删除文本框中的一个字符
    else if(strcmp(txt, LV_SYMBOL_OK) == 0) 
    {
        const char *text=lv_textarea_get_text(ta); // 获取文本框中的文本
        // 校验取件码
        for(int i=0;i<4;i++)
        {
            if(strcmp(text, Claim_data[i].Claim_code)== 0)
            {
                strcpy(Claim_data[i].Claim_code,"");
                strcpy(Claim_data[i].phone_number,"");
                Claim_data[i].is_used=false;
                Box_id=Claim_data[i].Box_ID;
                break;
            }
        }
        if (Box_id==0)
        {
            Msgbox_Show("警告：", "取件码无效！");
            lv_textarea_set_text(ta, "");
            return;
        }
        // 发送MQTT消息
        Send_Box_ID(Box_id);
        char str1[50];
        sprintf(str1,"%d号箱门已打开，请取件",Box_id);
        //    Serial.println(box_id);
        Msgbox_Show("提示",str1);
        client.publish("xiaoyan", text);
        // 清除输入框内容
        lv_textarea_set_text(ta, "");
        // 弹出提示框
    }
    else lv_textarea_add_text(ta, txt); 
}


// 创建用户界面元素的函数
void UserGet(lv_obj_t * parent)
{
    // 创建一个标签并设置文本
    lv_obj_t * label = lv_label_create(parent);
    lv_label_set_text(label, "请输入取件码：");

    // 创建一个文本区域并设置为单行模式
    lv_obj_t * ta = lv_textarea_create(parent);
    Set_default_font(ta);
    lv_textarea_set_one_line(ta, true);
    lv_obj_align(ta, LV_ALIGN_TOP_MID, 0, 30);
    lv_obj_add_event_cb(ta, textarea_event_handler, LV_EVENT_READY, ta);
    lv_obj_add_state(ta, LV_STATE_FOCUSED); /*To be sure the cursor is visible*/

    // 定义按钮矩阵的按钮布局
    static const char * btnm_map[] = {"1", "2", "3", "\n",
                                      "4", "5", "6", "\n",
                                      "7", "8", "9", "\n",
                                      LV_SYMBOL_BACKSPACE, "0", LV_SYMBOL_OK, ""
                                     };

    // 创建按钮矩阵并设置大小和位置
    lv_obj_t * btnm = lv_btnmatrix_create(parent);
    Set_default_font(btnm);
    lv_obj_set_size(btnm, 250, 200);
    lv_obj_align(btnm, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_obj_add_event_cb(btnm, btnm_event_handler, LV_EVENT_VALUE_CHANGED, ta);
    lv_obj_clear_flag(btnm, LV_OBJ_FLAG_CLICK_FOCUSABLE); /*To keep the text area focused on button clicks*/
    lv_btnmatrix_set_map(btnm, btnm_map);
}


