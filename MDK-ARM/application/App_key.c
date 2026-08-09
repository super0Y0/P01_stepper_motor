#include "App_key.h"
#include "stepper.h"

#define CIRCLE_MAX 20 // 最大圈数

extern uint8_t key_state; // 声明外部变量key_state
extern uint8_t display_flag; // 声明外部变量dispaly_flag
extern int8_t set_circle; // 声明外部变量set_circle
extern uint8_t machine_ID; // 声明外部变量machine_ID

/**
 * @brief 根据不同按键，展示不同功能
 *必须在循环里调用
 * KEY1:圈数加一 （id页中，KEY1为id加一）
 * KEY2:圈数减一 （id页中，KEY2为id减一）
 * KEY3:启动/停止 (id页中，KEY3为保存id)
 * KEY4:查看id
 * 
 */

void App_KeyFunction(void){
    if(display_flag){//在ID页面
        switch(key_state){
        case 1:
            key_state = 0; // 重置按键状态
            machine_ID++; // KEY1:ID加一
            break;
        case 2:
           key_state = 0; // 重置按键状态
           machine_ID--; // KEY2:ID减一
            break;
        case 3:
            key_state = 0; // 重置按键状态
            W24C02_WriteByte(0x01, machine_ID); // KEY3:保存ID
            break;
        case 4:
            key_state = 0; // 重置按键状态
            display_flag = 0; 
            Inf_OLED_Clear();
            break;
    }
}
    else if(!display_flag){//在主页面
        switch(key_state){
        case 1:
            key_state = 0; // 重置按键状态
            set_circle++; // KEY1:圈数加一
            if(set_circle > CIRCLE_MAX) { // 限制最大圈数为20
                set_circle = CIRCLE_MAX;
            }
            break;
        case 2:
           key_state = 0; // 重置按键状态
           set_circle--; // KEY2:圈数减一
            if(set_circle < -CIRCLE_MAX) { // 限制最小圈数为-20
                set_circle = -CIRCLE_MAX;
            }
            break;
        case 3:
            key_state = 0;
            Stepper_Config(set_circle); // KEY3:启动/停止
            break;
        case 4:
            key_state = 0; // 重置按键状态
            display_flag = 1; 
            Inf_OLED_Clear();
            break;
      }
    }
}
