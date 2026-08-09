#include "com_debug.h"

/**
 * @brief 日志输出初始化
 * 
 */
void Com_debug_init(void)
{
   //MX_USART1_UART_Init();

    //printf("com_debug_init success\r\n");

}

int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
    return ch;
}
