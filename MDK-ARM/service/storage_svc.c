#include "storage_svc.h"

uint8_t machine_ID = 5;//所有的从设备ID都存储在EEPROM中，第一次启动时，默认ID为1，不能直接用于控制使用

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
