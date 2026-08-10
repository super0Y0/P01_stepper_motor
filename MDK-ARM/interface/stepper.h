#ifndef __STEPPER_H
#define __STEPPER_H

#include "tim.h"
#include "math.h"

//梯形算法结构体
typedef struct{ 
    uint32_t target_step; //目标步数
    uint8_t dir; //旋转方向

    float acc; //加速阶段加速度
    float dec; //减速阶段加速度
    float v_max; //最大速度
    float v_min; //最小速度

    float current_speed; //当前速度
    uint32_t current_step; //当前步数
    uint8_t is_running; //是否在运行
    uint8_t state; //电机状态 0停止 1加速 2匀速 3减速
    
}Stepper_motor_Struct;


void Stepper_Start(uint8_t dir);
void Stepper_Config(int8_t set_num);
void Stepper_Stop(void);


#endif
