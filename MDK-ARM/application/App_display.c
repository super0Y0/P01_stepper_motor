#include "FreeRTOS.h"
#include "main.h"
#include "cmsis_os2.h"
#include "app_state.h"

void StartDisplayTask(void *argument)
{
  /* USER CODE BEGIN StartDisplayTask */
  
  App_OLED_Init();
  
  /* Infinite loop */
  for(;;)
  {
    osMutexAcquire(i2cMutexHandle, osWaitForever); // 获取互斥锁，确保OLED显示任务独占访问
    App_OLED_Show();
    osMutexRelease(i2cMutexHandle); // 释放互斥锁，允许其他任务访问I2C总线
    osDelay(100);
  }
  /* USER CODE END StartDisplayTask */
}
