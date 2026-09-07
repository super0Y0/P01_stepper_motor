#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "mb.h"
#include "stepper.h"
#include "storage_svc.h"
#include "app_state.h"

void App_Modbus_Run(void){
    //--- 1. 启动：线圈=1 且电机空闲时启动（转完自动停；Stepper_Stop 会把线圈清0，避免重复启动）---
    if(REG_COILS_BUF[2] == 1 && !stepper_motor.is_running){
        Stepper_Config(REG_HOLD_BUF[2] - 500); //配置电机参数并启动
    }
    //--- 2. 停止：线圈=0 且电机运行中则立即停止 ---
    if(REG_COILS_BUF[2] == 0 && stepper_motor.is_running){
        Stepper_Stop();
    }

    //--- 3. 同步圈数设置 ---
    if(REG_HOLD_BUF[2] != set_circle + 500){
        set_circle =  REG_HOLD_BUF[2] - 500; //设置电机圈数
    }

    //--- 4. 状态回显到只读离散量 ---
    REG_DISC_BUF[3] = stepper_motor.dir;                    //方向状态
    REG_DISC_BUF[4] = stepper_motor.is_running ? 1 : 0;     //运行状态
}

void StartModbusTask(void *argument)
{
  /* USER CODE BEGIN StartModbusTask */
 App_ID_Init(); // 初始化ID判断

 //初始化momdbus
 eMBInit(MB_RTU,machine_ID,0,115200,MB_PAR_NONE); 

 //使能modbus
  eMBEnable();
  /* Infinite loop */
  for(;;)
  {
    eMBPoll();
    App_Modbus_Run(); // 调用Modbus运行函数

    osDelay(50);
  }
  /* USER CODE END StartModbusTask */
}
