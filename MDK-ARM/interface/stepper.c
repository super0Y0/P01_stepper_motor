#include "stepper.h"
#include "stdlib.h"
#include "com_debug.h"

#define NUMBER_OF_STEPPER 3200 // 步进电机每转一圈的脉冲数

uint32_t target_num; //目标的步数
uint32_t current_num; //当前的步数

/**
 * @brief 启动电机
 * @param dir: 电机旋转方向 1正转 0反转
 *
 * 电机启动与三个引脚有关：DIR, SD, 输入脉冲
 */
void Stepper_Start(uint8_t dir){
    HAL_GPIO_WritePin(MOTOR_DIR_GPIO_Port, MOTOR_DIR_Pin, (GPIO_PinState)dir);
    HAL_GPIO_WritePin(MOTOR_SD_GPIO_Port, MOTOR_SD_Pin, GPIO_PIN_SET);
    
    // 产生输入脉冲
    __HAL_TIM_ENABLE_IT(&htim1,TIM_IT_UPDATE);
    HAL_TIM_OC_Start_IT(&htim1, TIM_CHANNEL_1);
}

/**
 * @brief 停止电机
 * 
 */
void Stepper_Stop(void){

    //频繁开关会导致不稳定，故停止时不关闭SD引脚，直接停止脉冲输出即可
    //HAL_GPIO_WritePin(MOTOR_SD_GPIO_Port, MOTOR_SD_Pin, GPIO_PIN_RESET);
    
    
    HAL_TIM_OC_Stop_IT(&htim1, TIM_CHANNEL_1);    

}

/**
 * @brief 配置电机参数
 * @param set: 设置的参数
 */
 void Stepper_Config(int8_t set_num){
   
   

    __HAL_TIM_SET_AUTORELOAD(&htim1, 624);
  
    uint8_t motor_dir = set_num > 0 ? 1 : 0;
    target_num = abs(set_num) * NUMBER_OF_STEPPER;
    Stepper_Start(motor_dir);

 }

 
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    if(htim->Instance == TIM1){
        current_num++;
        if(current_num >= target_num){
            Stepper_Stop();
            current_num = 0;
        }
    }
}
