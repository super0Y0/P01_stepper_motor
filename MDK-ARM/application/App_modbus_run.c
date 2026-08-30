#include "App_modbus_run.h"

extern int8_t set_circle;

void App_Modbus_Run(void){
    //判断电机是否启动
    if(REG_COILS_BUF[2] == 1){
        //电机启动
    Stepper_Config(REG_HOLD_BUF[2] - 500); //配置电机参数
    } 
    if(REG_HOLD_BUF[2] != set_circle + 500){
    set_circle =  REG_HOLD_BUF[2] - 500; //设置电机圈数
		}
}