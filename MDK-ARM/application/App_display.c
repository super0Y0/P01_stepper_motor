#include "App_display.h"

uint8_t machine_ID = 5;//所有的从设备ID都存储在EEPROM中，第一次启动时，默认ID为1，不能直接用于控制使用
uint8_t display_flag = 0; //显示标志位，0为主页面，1为ID页面
//记录需要运行的圈数
int8_t set_circle = 0;


/**
 * @brief 使用wc24c02来永久化存储从设备ID
 * 选择一个字节地址来存储id  => 0x01
 *还需要选择一个地址来存储验证码 => 0x00  -> 88
 *
 *验证流程：先读取验证码，如果验证码正确，则读取ID，如果不正确，可能为第一次启动-> 写入验证码，再写入初始id
 */
void App_ID_Init(void){
    //读取验证码
    uint8_t new_flag = W24C02_ReadByte(0x00);
    //判断验证码
    if(new_flag != 88){
        //第一次启动，写入验证码
        W24C02_WriteByte(0x00, 88);
        //写入初始id
        W24C02_WriteByte(0x01, machine_ID);
    } else {
        //非第一次启动，读取ID
        uint8_t saved_id = W24C02_ReadByte(0x01);
        //合法性检查：modbus从机地址合法范围1~247，非法(如下溢写入的252)则恢复默认ID
        if(saved_id == 0 || saved_id > 247){
            machine_ID = 5; //恢复默认ID
            W24C02_WriteByte(0x01, machine_ID); //重新写回，纠正EEPROM中的非法值
        } else {
            machine_ID = saved_id;
        }
    }

    printf("ID: %d\n", machine_ID);
}

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
 HAL_Delay(10);
}
