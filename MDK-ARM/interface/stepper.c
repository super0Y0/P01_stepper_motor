#include "stepper.h"
#include "stdlib.h"
#include "com_debug.h"

#define NUMBER_OF_STEPPER 1600 // 步进电机每转一圈的脉冲数 (步数)
#define MOTOR_MAX_SPEED 2000 // 最大速度

// 记录当前的步间隔
uint32_t step_interval = 0;
// 记录余数
uint32_t step_remainder = 0;

Stepper_motor_Struct stepper_motor = { 
    .target_step = 0,//目标步数
    .dir = 0, //旋转方向

    .acc = 800, //加速阶段加速度  常量 单位均为步/秒^2
    .dec = 800, //减速阶段加速度  常量
    .v_max = MOTOR_MAX_SPEED, //最大速度 三角形时不一定能达到最大速度
    .v_min = 50, //最小速度  常量

    .current_speed = 0, //当前速度
    .current_step = 0, //当前步数
    .is_running = 0, //是否在运行 0停止，1运行
    .state = 0, //电机状态 0停止 1加速 2匀速 3减速
    
};

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

//计算下一次的步间隔
uint32_t Stepper_CalculateNextStep(void)
{
    // 1. 计算剩余步数
    uint32_t remain_step = stepper_motor.target_step - stepper_motor.current_step;
    // 2. 计算加速阶段步数和减速阶段步数
    uint32_t acc_step = stepper_motor.v_max * stepper_motor.v_max / (stepper_motor.acc * 2);
    uint32_t dec_step = stepper_motor.v_max * stepper_motor.v_max / (stepper_motor.dec * 2);

    // 3. 判断当前状态 => 计算下一个步间隔的速度
    // 加速阶段判断
    if (stepper_motor.current_step < acc_step && stepper_motor.state == 0)
    {
        // 3.1 处于加速阶段 => 计算下一个步间隔的速度
        stepper_motor.current_speed += (stepper_motor.acc * 1.0 / stepper_motor.current_speed);

        // 3.2 判断是否会进入下一个状态
        if (stepper_motor.current_speed >= stepper_motor.v_max)
        {
            // 进入下一个状态 => 匀速阶段
            stepper_motor.state = 1; // 匀速阶段
            stepper_motor.current_speed = stepper_motor.v_max;
        }
    }
    // 匀速阶段判断
    else if (remain_step > dec_step && stepper_motor.state == 1)
    {
        // 3.3 处于减速阶段 => 计算下一个步间隔的速度
        stepper_motor.current_speed = stepper_motor.v_max;
    }
    // 减速阶段
    else
    {   
        stepper_motor.state = 2;
        // 3.4 处于减速阶段 => 计算下一个步间隔的速度
        stepper_motor.current_speed -= (stepper_motor.dec * 1.0 / stepper_motor.current_speed);
    }

    // 4. 设置速度的上下限
    if (stepper_motor.current_speed > stepper_motor.v_max)
    {
        stepper_motor.current_speed = stepper_motor.v_max;
    }
    else if (stepper_motor.current_speed < stepper_motor.v_min)
    {
        stepper_motor.current_speed = stepper_motor.v_min;
    }

    return (uint32_t)(1000000.0 / stepper_motor.current_speed);
}




/**
 * @brief 配置电机参数
 * @param set: 设置的参数
 * @note：使用梯形算法
 */
 void Stepper_Config(int8_t set_num){
   if(stepper_motor.is_running){
     return;
   } 
   stepper_motor.dir = (set_num > 0) ? 1 : 0;
   stepper_motor.target_step = abs(set_num * NUMBER_OF_STEPPER);
   //设置默认最大数度
    stepper_motor.v_max = MOTOR_MAX_SPEED;
    //计算加速阶段与减速阶段的步数
    uint32_t acc_step = stepper_motor.v_max * stepper_motor.v_max / (stepper_motor.acc * 2);
    uint32_t dec_step = stepper_motor.v_max * stepper_motor.v_max / (stepper_motor.dec * 2);
    // 判断当前设置的目标步数是否能达到默认的最大速度
    if (stepper_motor.target_step < acc_step + dec_step)
    {
        //达不到设置的默认最大速度 => 三角形算法  => 重新计算最大速度
        stepper_motor.v_max = sqrt((2.0 * stepper_motor.target_step * stepper_motor.acc * stepper_motor.dec) / (stepper_motor.acc + stepper_motor.dec));
    }
  //初始化其他参数且说明电机开始运行
    stepper_motor.state = 0; // 加速阶段
    stepper_motor.current_step = 0;
    stepper_motor.current_speed = 0;
    stepper_motor.is_running = 1;
    //计算下一次的步间隔
        step_interval = Stepper_CalculateNextStep();
        step_remainder = step_interval % 2;
     //启动电机
     __HAL_TIM_SET_COUNTER(&htim1, 0);
     __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (step_interval / 2));
    Stepper_Start(stepper_motor.dir);


 }

 uint8_t double_flag = 0;

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim){
   if(htim ->Instance == TIM1){
    if(!double_flag){
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, ((step_interval + step_remainder) / 2) + __HAL_TIM_GET_COMPARE(&htim1, TIM_CHANNEL_1)); 
        double_flag = 1;
    }else if(double_flag){
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (step_interval / 2) + __HAL_TIM_GET_COMPARE(&htim1, TIM_CHANNEL_1));
        double_flag = 0;
        step_interval = Stepper_CalculateNextStep();
        step_remainder = step_interval % 2;
        stepper_motor.current_step++;
        if(stepper_motor.current_step >= stepper_motor.target_step){
            Stepper_Stop();
            stepper_motor.is_running = 0;
        }
    }
    

   }
}
