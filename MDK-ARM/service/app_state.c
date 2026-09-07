#include "app_state.h"
#include "storage_svc.h"
#include "Int_oled.h"

uint8_t display_flag = 0; //显示标志位，0为主页面，1为ID页面
//记录需要运行的圈数
int8_t set_circle = 0;

void App_OLED_Init(void){
    Inf_OLED_Init();
}

/**
 * @brief ID修改及圈数显示
 * 
 * 1.主页面 展示圈数set及当前的状态state  此页面下可进行圈数的设置
 * 2.ID页面 进行ID设置
 * flag = 0.主页面   1.ID页面
 */
void App_OLED_Show(void){
   
    if(display_flag){ 
        
        Inf_OLED_ShowString(30, 16, "ID:", 16, 1);
        Inf_OLED_ShowNum(60, 16, machine_ID, 3, 16, 1);
}else if (!display_flag){
   
   for(uint8_t i=3;i<7;i++)
  {
    Inf_OLED_ShowChinese( i * 18 - 3*18 + 28, 10, i,16,1);
    
  }
    Inf_OLED_ShowString(10, 32, "set:", 16, 1);
    //展示负数实现正反转
    if(set_circle < 0){
        //先展示负号,用X代替
        Inf_OLED_ShowChar(50, 32, 'X', 16, 1);
        Inf_OLED_ShowNum(60, 32, -set_circle, 3, 16, 1);
    }else{
        Inf_OLED_ShowChar(50, 32, ' ', 16, 1);
        Inf_OLED_ShowNum(60, 32, set_circle, 3, 16, 1);
    }

 }
 Inf_OLED_Refresh();

}
