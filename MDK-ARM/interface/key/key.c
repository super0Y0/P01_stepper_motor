#include "key.h"
#include "FreeRTOS.h"
#include "cmsis_os2.h"

#define KEY_DEBOUNCE_MS 5 // 按键消抖延时，单位：毫秒
#define IS_KEY1_PRESSED() (HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == GPIO_PIN_RESET)
#define IS_KEY2_PRESSED() (HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin) == GPIO_PIN_RESET)
#define IS_KEY3_PRESSED() (HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin) == GPIO_PIN_RESET)
#define IS_KEY4_PRESSED() (HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY4_Pin) == GPIO_PIN_RESET)

uint8_t key_state = 0; // 按键状态变量
static uint8_t key_pressed[4] = {0};   // 每键是否处于"已按下未释放"状态

static uint8_t key_clicked(uint8_t idx, GPIO_TypeDef *port, uint16_t pin)
{
    uint8_t pressed_now = (HAL_GPIO_ReadPin(port, pin) == GPIO_PIN_RESET);

    // 按下沿（首帧）：延时消抖后二次确认，确认成功才报一次"点击"
    if (pressed_now && !key_pressed[idx]) {
        osDelay(KEY_DEBOUNCE_MS);
        if (HAL_GPIO_ReadPin(port, pin) == GPIO_PIN_RESET) {
            key_pressed[idx] = 1;
            return 1;
        }
    }
    // 释放沿：复位状态，准备下一次点击
    if (!pressed_now) {
        key_pressed[idx] = 0;
    }
    return 0;
}

void Key_Scan(void)
{
    if      (key_clicked(0, KEY1_GPIO_Port, KEY1_Pin)) key_state = 1;
    else if (key_clicked(1, KEY2_GPIO_Port, KEY2_Pin)) key_state = 2;
    else if (key_clicked(2, KEY3_GPIO_Port, KEY3_Pin)) key_state = 3;
    else if (key_clicked(3, KEY4_GPIO_Port, KEY4_Pin)) key_state = 4;
}
